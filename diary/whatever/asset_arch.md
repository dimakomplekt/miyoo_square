# Guideline: Архитектура системы ассетов

## 1. Цель

Система ассетов должна обеспечить безопасное, понятное и расширяемое использование ресурсов движка через несколько уровней абстракции:

- `Asset`
- `AssetInstance`
- `Handle`
- `My_SDL_Texture`
- игровые объекты (`Character` и т.д.)

На первом этапе реализуется только работа с изображениями.

При этом архитектура должна быть построена так, чтобы в дальнейшем та же концепция могла использоваться для других типов ресурсов:

- `Audio`;
- `Font`;
- `Animation`;
- другие типы ассетов.

Основная идея системы:

```text
Asset
    ↓
AssetInstance
    ↓
Handle
    ↓
Engine component / My_SDL_Texture
    ↓
Game Object
```

Главная задача архитектуры заключается не в том, чтобы полностью исключить raw pointer, а в том, чтобы разделить:

- **ownership** — кто отвечает за lifetime;
- **usage** — кто использует объект;
- **identification** — как объект идентифицируется;
- **access** — как получить быстрый raw pointer для работы с объектом.

---

# 2. Четыре основных правила

## Правило №1. `AssetManager` владеет `Asset`

`Asset` является базовым ресурсом проекта.

Например:

```text
AssetManager
    ├── ImageAsset
    ├── ImageAsset
    └── ImageAsset
```

Только `AssetManager` отвечает за создание и уничтожение `Asset`.

Другие объекты могут использовать `Asset`, но не должны самостоятельно управлять его lifetime.

Игровой объект, GUI или `AssetInstance` не должны делать:

```cpp
delete asset;
```

или каким-либо другим способом самостоятельно уничтожать `Asset`.

### Принцип

> Кто владеет объектом, тот отвечает за его lifetime.

Это правило устраняет необходимость определять владельца `Asset` среди всех его пользователей.

---

## Правило №2. `InstanceManager` владеет `AssetInstance`

`AssetInstance` является runtime-представлением конкретного использования `Asset`.

Например:

```text
ImageAsset
    ↓
ImageInstance
    ├── SDL_Texture #1
    ├── SDL_Texture #2
    ├── SDL_Texture #3
    └── custom runtime data
```

`AssetInstance` может содержать:

- `Handle` базового `Asset`;
- созданные runtime-ресурсы;
- `SDL_Texture`;
- состояние анимации;
- текущий кадр;
- параметры конкретного экземпляра;
- другие runtime-данные.

`InstanceManager` является владельцем `AssetInstance`.

Следовательно:

- `Character` не удаляет `AssetInstance`;
- `My_SDL_Texture` не удаляет `AssetInstance`;
- GUI не удаляет `AssetInstance`;
- другие пользователи не удаляют `AssetInstance`.

И снова действует одно правило:

> Пользователь ресурса не является его владельцем.

---

## Правило №3. Пользователи ресурсов не владеют ресурсами менеджеров

`Character`, `My_SDL_Texture`, GUI и другие объекты используют ресурсы, но не определяют их lifetime.

Например:

```text
Character
    └── ImageInstanceHandle

My_SDL_Texture
    └── ImageInstanceHandle
```

Несколько объектов могут одновременно использовать один `AssetInstance`:

```text
                    ImageInstance
                   /      |      \
                  /       |       \
                 ↓        ↓        ↓
             Texture A Texture B Character
```

Это не означает совместное владение.

Владелец остаётся один:

```text
InstanceManager
    ↓ owns
ImageInstance
```

Остальные объекты только используют `ImageInstance`.

Таким образом, не требуется определять:

```cpp
bool owns_instance;
```

для каждого пользователя.

---

## Правило №4. Raw pointer используется только внутри безопасной границы

Обычные C++ указатели разрешены и должны использоваться там, где это удобно и быстро.

Однако raw pointer не является механизмом lifetime.

Безопасная схема выглядит так:

```text
Handle
    ↓
Manager::get(handle)
    ↓
проверка существования
    ↓
ImageInstance*
    ↓
обычная работа с объектом
```

Например:

```cpp
ImageInstance* instance = instance_manager.get(handle);

if (!instance)
    return;

instance->update();
```

После получения валидного pointer можно использовать обычный C++ API.

Главное правило:

> Raw pointer не должен быть единственным источником информации о том, существует ли объект.

Иными словами:

```text
Handle
    = идентификация и проверка

Manager
    = контроль lifetime

Raw pointer
    = быстрый доступ после проверки
```

---

# 3. Общая архитектура

Основная архитектура системы:

```text
                         ENGINE
                           │
             ┌─────────────┴─────────────┐
             │                           │
             ↓                           ↓
       AssetManager               InstanceManager
             │                           │
           owns                        owns
             │                           │
             ↓                           ↓
        ImageAsset                  ImageInstance
             ↑                           │
             │                           │
           Handle                      Handle
             │                           │
             └──────────────┐    ┌───────┘
                            │    │
                            ↓    ↓
                          Users
                            │
              ┌─────────────┼─────────────┐
              ↓             ↓             ↓
          Character      GUI element   My_SDL_Texture
```

Главное разделение ответственности:

```text
Asset
    = ресурс, существующий в проекте

AssetInstance
    = runtime-представление конкретного использования Asset

Handle
    = безопасный идентификатор объекта

My_SDL_Texture
    = компонент, использующий ImageInstance

Character
    = игровой объект, использующий ресурсы
```

---

# 4. `Asset`

`Asset` представляет базовый ресурс проекта.

Для изображения это может быть:

```text
ImageAsset
    ├── build path
    ├── сведения об изображении
    ├── данные загрузки
    └── asset-specific data
```

Важно разделять путь исходного файла и путь ресурса в готовом Build.

`Asset` должен представлять ресурс с точки зрения движка, а не текущей файловой системы разработчика.

Например, исходный файл может находиться в:

```text
D:/Projects/MyGame/assets/images/hero.png
```

а в Build ресурс может находиться по совершенно другому пути.

Поэтому `Asset` должен оперировать понятием ресурса движка, а не напрямую зависеть от текущего расположения исходного файла.

`Asset` не должен знать:

- какой `Character` его использует;
- какой GUI-элемент его использует;
- какая `My_SDL_Texture` его отображает;
- сколько пользователей у него сейчас.

Его основная задача:

> Представлять и хранить базовый ресурс.

---

# 5. `AssetInstance`

`AssetInstance` представляет конкретное runtime-использование `Asset`.

Например, один файл:

```text
hero.png
```

может использоваться для создания нескольких независимых экземпляров:

```text
hero.png
    ├── IdleInstance
    ├── RunInstance
    ├── JumpInstance
    └── AttackInstance
```

Каждый `Instance` может иметь собственное runtime-состояние.

Для изображения это может выглядеть так:

```text
ImageInstance
    ├── AssetHandle
    ├── SDL_Texture[]
    ├── current frame
    ├── animation state
    └── custom data
```

Получается следующая модель:

```text
один Asset
    ↓
много AssetInstance
    ↓
каждый Instance имеет собственное runtime-состояние
```

Это позволяет использовать один исходный ресурс в разных контекстах независимо друг от друга.

Например:

```text
hero.png
    │
    ├── Character 1 → Instance A
    │
    ├── Character 2 → Instance B
    │
    └── GUI preview → Instance C
```

Все три экземпляра используют один базовый ресурс, но могут иметь совершенно разное состояние.

---

# 6. Почему `Asset` и `AssetInstance` разделяются

`Asset` и `AssetInstance` решают разные задачи.

## `Asset`

Отвечает на вопрос:

> Какой ресурс существует в проекте?

Например:

```text
hero.png
```

## `AssetInstance`

Отвечает на вопрос:

> Как именно этот ресурс используется сейчас?

Например:

```text
HeroRunInstance
    ├── current frame = 3
    ├── animation time = ...
    ├── SDL_Texture[]
    └── другие runtime-данные
```

Это позволяет одному `Asset` иметь несколько независимых `Instance`.

Например:

```text
hero.png
    │
    ├── Character 1 → Instance A
    │
    ├── Character 2 → Instance B
    │
    └── GUI preview → Instance C
```

При этом `Instance A`, `Instance B` и `Instance C` не обязаны иметь одинаковое состояние.

Это особенно важно для анимаций.

Например два персонажа могут использовать один и тот же набор изображений, но находиться на разных кадрах:

```text
hero.png
    ↓
ImageAsset
    ├── Character A → RunInstance → frame 2
    └── Character B → RunInstance → frame 7
```

Базовый ресурс при этом остаётся один.

---

# 7. `Handle`

`Handle` используется вместо хранения постоянного raw pointer между подсистемами.

Минимальная концепция:

```cpp
struct ImageInstanceHandle
{
    uint32_t index;
    uint32_t generation;
};
```

Например:

```text
index      = 15
generation = 7
```

`Handle` не содержит адрес объекта.

Он содержит идентификатор объекта внутри `Manager`.

Это принципиально важно.

Если хранить:

```cpp
ImageInstance* instance;
```

то после удаления объекта pointer может остаться в памяти, но больше не указывать на существующий объект.

`Handle` вместо этого говорит:

```text
"Мне нужен объект №15 поколения 7"
```

А уже `InstanceManager` решает, существует ли такой объект в данный момент.

---

# 8. Как `Manager` хранит `Instance`

Концептуально `Manager` может иметь набор slots:

```text
InstanceManager
    ├── slot 0
    ├── slot 1
    ├── slot 2
    ├── ...
    └── slot 15
```

Каждый slot содержит примерно:

```text
InstanceSlot
    ├── ImageInstance
    ├── generation
    └── alive
```

`Handle`:

```text
{ index = 15, generation = 7 }
```

указывает на slot №15 и ожидает, что этот slot сейчас содержит объект поколения `7`.

Таким образом, `Handle` можно рассматривать как пару:

```text
index
    = где искать

generation
    = какой именно объект там должен находиться
```

Это позволяет безопасно переиспользовать память после удаления объектов.

---

# 9. Как `Handle` защищает от удалённого объекта

Рассмотрим ситуацию.

`Character` хранит:

```text
Handle {15, 7}
```

`InstanceManager` содержит:

```text
slot 15
    generation = 7
    alive = true
```

`Character` вызывает:

```cpp
instance_manager.get(handle);
```

`Manager` проверяет:

```text
slot существует?
    ↓
объект alive?
    ↓
generation совпадает?
```

Если все проверки успешны:

```text
ImageInstance*
```

возвращается пользователю.

Если объект уже был удалён или Handle устарел:

```text
nullptr
```

Таким образом, `Handle` сам по себе не является указателем на память.

Он является запросом к `Manager`:

> "Дай мне объект, который соответствует этому идентификатору, если он ещё существует."

---

# 10. Что происходит после удаления `Instance`

Допустим:

```text
Character
    ↓
Handle {15, 7}
```

`InstanceManager` содержит:

```text
slot 15
    generation = 7
    alive = true
```

Затем выполняется:

```cpp
instance_manager.destroy(handle);
```

`Instance` уничтожается.

Slot больше не представляет старый объект.

Например:

```text
slot 15
    generation = 8
    alive = false
```

Старый `Handle` при этом всё ещё содержит:

```text
{15, 7}
```

При следующем обращении:

```cpp
instance_manager.get({15, 7});
```

`Manager` сравнивает:

```text
Handle generation = 7
Slot generation   = 8
```

Значения не совпадают.

Результат:

```text
nullptr
```

Таким образом старый `Handle` становится невалидным.

Важно, что пользователь не должен самостоятельно определять:

```cpp
if (instance != nullptr)
```

для старого raw pointer.

Он снова обращается через `Handle`:

```cpp
ImageInstance* instance = instance_manager.get(handle);
```

и получает либо актуальный объект, либо `nullptr`.

---

# 11. Зачем нужен `generation`

`generation` защищает от повторного использования старого slot.

Например, существовал старый `Instance`:

```text
index = 15
generation = 7
```

После удаления slot №15 может быть повторно использован.

Позже `Manager` создаёт новый `Instance`:

```text
index = 15
generation = 8
```

Если бы `Handle` состоял только из:

```text
index = 15
```

старый пользователь мог бы случайно получить новый `Instance`.

Это потенциально очень опасная ошибка.

`generation` предотвращает её:

```text
Старый Handle:
    {15, 7}

Новый Slot:
    {15, 8}

7 != 8

→ Handle недействителен
```

Поэтому пара:

```text
index + generation
```

является существенно более безопасной системой идентификации runtime-объектов, чем один индекс.

---

# 12. `Handle` не является магической защитой

`Handle` защищает систему только при правильном использовании.

Нельзя сделать:

```cpp
ImageInstance* ptr;
```

и считать, что наличие `Handle` где-то рядом автоматически делает `ptr` безопасным.

Небезопасно:

```cpp
ImageInstance* ptr = instance_manager.get(handle);

// Где-то позже Instance был уничтожен.

// ptr всё ещё существует как адрес,
// но объект больше не существует.

ptr->update();
```

`ptr` уже является dangling pointer.

Безопасная архитектура выглядит иначе:

```text
ImageInstanceHandle
        ↓
InstanceManager::get()
        ↓
проверка
        ↓
ImageInstance*
        ↓
немедленная работа
```

Например:

```cpp
ImageInstance* instance = instance_manager.get(handle);

if (!instance)
    return;

instance->update();
```

Raw pointer желательно использовать как временный локальный доступ, полученный непосредственно перед работой.

---

# 13. Lifetime `Manager`

`Handle` не должен использоваться после уничтожения соответствующего `Manager`.

Поэтому система должна иметь чёткий lifetime.

Например:

```text
Engine
│
├── AssetManager
├── InstanceManager
│
└── Scene
     ├── Character
     ├── GUI
     └── My_SDL_Texture
```

Правильный порядок уничтожения:

```text
1. Scene
   ↓
   Character
   GUI
   My_SDL_Texture

2. InstanceManager
   ↓
   AssetInstance

3. AssetManager
   ↓
   Asset
```

Иными словами:

> Пользователи ресурсов должны уничтожаться раньше менеджеров, которые владеют этими ресурсами.

Это является отдельным архитектурным правилом.

`Handle` защищает от удаления конкретного объекта внутри живого `Manager`, но не обязан защищать от уничтожения самого `Manager`.

---

# 14. Почему `Handle` не обязан спасать от уничтожения `Manager`

Рассмотрим:

```text
AssetManager
    ↓
InstanceManager
    ↓
Scene
    ↓
Character
```

Если уничтожить `InstanceManager`, пока `Character` ещё жив, нарушается lifetime-контракт системы.

После этого `Character` физически не может безопасно вызвать:

```cpp
instance_manager.get(handle);
```

потому что самого `InstanceManager` уже нет.

`Handle` не должен превращать неправильную архитектуру lifetime в автоматически безопасную систему.

Поэтому используются два разных уровня защиты:

```text
1. Архитектура lifetime
   ↓
   Manager живёт дольше своих пользователей

2. Generation Handle
   ↓
   Защищает от устаревшего Handle
   и удаления конкретного Instance
```

Обе системы нужны, но они решают разные проблемы.

---

# 15. Удаление `Instance` во время жизни `Scene`

`Instance` может быть удалён до окончания жизни `Scene`.

Например:

```text
Character
    ↓
Handle {15, 7}
```

Затем:

```cpp
instance_manager.destroy(handle);
```

`Instance` удаляется.

`Character` продолжает существовать.

На следующем обращении:

```cpp
ImageInstance* instance =
    instance_manager.get(handle);
```

получаем:

```text
nullptr
```

`Character` должен обработать это состояние:

```cpp
if (!instance)
{
    // Instance больше не существует.
    return;
}
```

После проверки можно безопасно работать с pointer:

```cpp
instance->update();
```

При этом `Character` не обязан знать, почему `Instance` исчез.

Ему достаточно знать:

```text
Handle valid
    → можно использовать Instance

Handle invalid
    → Instance больше недоступен
```

Это существенно упрощает взаимодействие между подсистемами.

---

# 16. Деструкторы

Главный принцип:

> Деструктор не должен заниматься управлением чужими объектами.

Нежелательная схема:

```text
Texture destructor
    ↓
уведомить Instance

Instance destructor
    ↓
уведомить Asset

Character destructor
    ↓
удалить Texture
    ↓
удалить Instance
    ↓
удалить Asset
```

Такая система быстро превращается в граф взаимных зависимостей.

В ней становится сложно определить:

- кто действительно владеет объектом;
- кто имеет право его удалить;
- что произойдёт при нескольких пользователях;
- кто должен обновить чужие указатели;
- какой порядок деструкторов безопасен.

Предпочтительная схема:

```text
Manager
    ↓
owns object

User
    ↓
uses object

Handle
    ↓
идентифицирует object

Manager::get()
    ↓
проверяет object

Raw pointer
    ↓
используется после проверки
```

В результате деструкторы объектов становятся максимально простыми.

`Character` уничтожается как `Character`.

`My_SDL_Texture` уничтожается как `My_SDL_Texture`.

`AssetInstance` уничтожается `InstanceManager`.

`Asset` уничтожается `AssetManager`.

Не возникает цепочки:

```text
Character destructor
    ↓
Texture destructor
    ↓
Instance destructor
    ↓
Asset destructor
```

---

# 17. `My_SDL_Texture`

`My_SDL_Texture` является пользователем `ImageInstance`.

Он не должен владеть `Instance`.

Например:

```cpp
class My_SDL_Texture
{
    ImageInstanceHandle instance;
};
```

Когда требуется работа с `Instance`:

```cpp
ImageInstance* instance =
    instance_manager.get(this->instance);

if (!instance)
    return;

instance->set_frame(2);
```

После получения pointer используется обычный C++ API.

При этом `My_SDL_Texture` не должен знать, где физически хранится `ImageInstance`.

Ему достаточно иметь:

```text
ImageInstanceHandle
```

и доступ к соответствующему `InstanceManager`.

---

# 18. `Character`

`Character` может содержать несколько `InstanceHandle`.

Например:

```text
Character
    ├── Idle Instance
    ├── Run Instance
    ├── Jump Instance
    └── My_SDL_Texture
             └── Current Instance
```

В простом случае `Character` может переключать текущий Instance:

```cpp
texture.set_instance(run_1);
texture.set_instance(run_2);
texture.set_instance(run_3);
texture.set_instance(run_4);
```

Таким образом можно реализовать простую анимацию:

```text
Character
    ↓
My_SDL_Texture
    ↓
Instance A
    ↓
Instance B
    ↓
Instance C
    ↓
Instance D
```

При этом `Character` не обязан удалять эти `Instance`.

Он только хранит ссылки на нужные runtime-ресурсы и выбирает, какой Instance использовать в данный момент.

---

# 19. Один `Instance` может использоваться несколькими объектами

Например:

```text
                    ImageInstance
                   /      |      \
                  /       |       \
                 ↓        ↓        ↓
             Texture A Texture B Character
```

При этом:

```text
InstanceManager
    ↓ owns
ImageInstance
```

а:

```text
Texture A
Texture B
Character
```

только используют `Instance`.

Не требуется:

```cpp
owns_instance = true;
owns_instance = false;
```

у каждого пользователя.

Количество пользователей вообще не должно влиять на ownership.

Если один `Instance` используется одним объектом:

```text
InstanceManager
    ↓ owns
Instance
    ↓
Texture
```

Если десятью:

```text
InstanceManager
    ↓ owns
Instance
    ├── Texture A
    ├── Texture B
    ├── Character A
    ├── Character B
    └── GUI
```

Владелец всё равно один.

---

# 20. Почему не следует использовать `owns_*` флаги

Не следует строить lifetime-систему на наборе:

```cpp
bool owns_asset;
bool owns_instance;
bool owns_texture;
```

Особенно опасной такая схема становится при нескольких пользователях:

```text
                    Instance
                 /     |      \
                /      |       \
               ↓       ↓        ↓
          Texture A Texture B Character
```

Сразу возникает множество вопросов:

- кто настоящий владелец;
- кто удаляет объект;
- что делать при удалении одного пользователя;
- что делать при наличии нескольких пользователей;
- когда последний пользователь исчез;
- кто должен менять `owns`;
- может ли один пользователь удалить объект, которым пользуются остальные.

Централизованное владение значительно проще:

```text
InstanceManager
    ↓
owns Instance
```

Остальные объекты:

```text
    ↓
use Instance
```

Таким образом, ownership является свойством архитектуры, а не состоянием каждого пользователя.

---

# 21. `Asset` и `Instance` не должны превращаться в граф взаимного владения

Не следует строить такую модель:

```text
Asset
    ↓ owns
Instance
    ↓ owns
Texture
    ↓ owns
Character
    ↓ owns
Instance
    ↓
Asset
```

Это создаёт циклическую модель lifetime.

Вместо этого владение должно быть однонаправленным:

```text
AssetManager
    ↓ owns
Asset

InstanceManager
    ↓ owns
Instance

Character / GUI / Texture
    ↓ use
Instance

Instance
    ↓ use
Asset
```

Здесь очень важно различать:

```text
owns
```

и:

```text
uses
```

`Instance` может использовать `Asset`, но это не означает, что `Instance` владеет `Asset`.

`Character` может использовать `Instance`, но это не означает, что `Character` владеет `Instance`.

Именно это разделение предотвращает появление сложного графа взаимных деструкторов.

---

# 22. `My_SDL_Texture` и `Asset`

Основной путь рекомендуется строить через `Instance`:

```text
My_SDL_Texture
        ↓
ImageInstance
        ↓
ImageAsset
```

Таким образом `My_SDL_Texture` получает уже подготовленный runtime-ресурс.

Если требуется доступ к базовому `Asset`, его можно получить через `Instance`:

```text
ImageInstance
    ↓
AssetHandle
    ↓
AssetManager
    ↓
ImageAsset*
```

Это сохраняет единую систему lifetime и проверки.

Основной сценарий остаётся:

```text
Texture
    ↓
Instance
    ↓
Asset
```

а не:

```text
Texture
    ├── Instance
    └── Asset
```

Это уменьшает количество независимых связей внутри `My_SDL_Texture`.

---

# 23. Возможность прямого использования `Asset`

В некоторых случаях может потребоваться использование `Asset` непосредственно через `My_SDL_Texture`.

Такой режим допустим, если он действительно нужен архитектуре.

Например:

```text
My_SDL_Texture
    ↓
AssetHandle
    ↓
AssetManager
    ↓
ImageAsset
```

Однако должны сохраняться те же правила:

- `My_SDL_Texture` не владеет `Asset`;
- `Asset` уничтожается только `AssetManager`;
- обращение происходит через `Manager`;
- raw pointer используется только после проверки;
- lifetime `AssetManager` должен превышать lifetime пользователей.

Прямой доступ к `Asset` не должен создавать отдельную систему управления памятью.

---

# 24. Общий принцип для `Audio`

Архитектура должна быть независима от типа ресурса.

Для `Audio` можно использовать ту же модель:

```text
AudioAssetManager
        ↓
    AudioAsset
        ↓
AudioInstanceManager
        ↓
   AudioInstance
        ↓
    AudioPlayer
```

Аналогично:

```text
AudioAssetHandle
AudioInstanceHandle
```

Получается единая концепция:

```text
Asset
    ↓
Instance
    ↓
Handle
    ↓
Engine component
    ↓
Game Object
```

Для изображения:

```text
ImageAsset
    ↓
ImageInstance
    ↓
ImageInstanceHandle
    ↓
My_SDL_Texture
    ↓
Character
```

Для аудио:

```text
AudioAsset
    ↓
AudioInstance
    ↓
AudioInstanceHandle
    ↓
AudioPlayer
    ↓
Character
```

Это позволяет не создавать отдельную архитектуру lifetime для каждого типа ресурсов.

Меняется тип данных:

```text
ImageAsset
AudioAsset
FontAsset
```

но принцип владения и доступа остаётся одинаковым.

---

# 25. Рекомендуемая структура системы

На концептуальном уровне:

```text
Engine
│
├── AssetManager
│    └── Assets
│
├── InstanceManager
│    └── Instances
│
└── Scene
     ├── Game Objects
     ├── GUI
     └── Engine components
```

Связи между объектами:

```text
AssetManager
    │ owns
    ▼
Asset

InstanceManager
    │ owns
    ▼
AssetInstance
    │ uses
    ▼
Asset

Game Object
    │ uses
    ▼
AssetInstance

My_SDL_Texture
    │ uses
    ▼
AssetInstance
```

Важный момент:

`Manager` не обязан знать о каждом пользователе `Instance`.

Например, `InstanceManager` не обязан хранить:

```text
Instance
    ├── Texture A
    ├── Texture B
    └── Character
```

Он может вообще не знать, кто использует объект.

Его ответственность значительно проще:

```text
создать Instance
    ↓
хранить Instance
    ↓
выдать Handle
    ↓
проверить Handle
    ↓
вернуть Instance
    ↓
уничтожить Instance
```

Это существенно уменьшает связанность системы.

---

# 26. Минимальный API

Первая реализация должна быть максимально маленькой.

Для `Asset`:

```cpp
AssetHandle load(...);

Asset* get(AssetHandle);
```

Для `Instance`:

```cpp
ImageInstanceHandle create(...);

ImageInstance* get(ImageInstanceHandle);

void destroy(ImageInstanceHandle);
```

Для `Texture`:

```cpp
void set_instance(ImageInstanceHandle);

ImageInstance* get_instance();
```

Не нужно сразу добавлять:

- сложную систему событий;
- `Observer`;
- `shared_ptr`;
- автоматическое отслеживание всех пользователей;
- сложные reference counters;
- систему глобальных callbacks;
- большое количество ownership-флагов.

Сначала должна заработать базовая модель lifetime:

```text
Manager owns
    ↓
User stores Handle
    ↓
Manager validates Handle
    ↓
User receives pointer
    ↓
User works with object
```

После этого дополнительные механизмы можно добавлять только при появлении реальной необходимости.

---

# 27. Первый прототип

Рекомендуемый порядок реализации:

```text
1. ImageAsset

2. AssetManager

3. ImageInstance

4. InstanceManager

5. ImageInstanceHandle

6. My_SDL_Texture

7. Character
```

После этого необходимо проверить несколько сценариев.

## Сценарий 1. Обычное использование

```text
Asset
    ↓
Instance
    ↓
Texture
    ↓
Character
```

Ожидаемое поведение:

```text
Handle valid
    ↓
Manager::get()
    ↓
Instance*
    ↓
Texture использует Instance
```

## Сценарий 2. Несколько пользователей

```text
Instance
    ├── Texture A
    ├── Texture B
    └── Character
```

Удаление одного пользователя не должно уничтожать `Instance`.

## Сценарий 3. Удаление `Instance`

```text
Character
    ↓
Handle

InstanceManager.destroy(handle)

Character
    ↓
get(handle)
    ↓
nullptr
```

`Character` продолжает существовать.

Он просто обнаруживает, что ресурс больше недоступен.

## Сценарий 4. Повторное использование slot

```text
Old:
    {15, 7}

delete

New:
    {15, 8}

Old Handle:
    {15, 7}

get(old_handle)
    ↓
nullptr
```

Это проверяет работу `generation`.

## Сценарий 5. Уничтожение `Scene`

```text
Scene
    ↓
Characters / GUI / Texture

InstanceManager
    ↓
Instances

AssetManager
    ↓
Assets
```

Сначала уничтожаются пользователи ресурсов.

После этого уничтожаются владельцы ресурсов.

---

# 28. Итоговая концепция

Система должна строиться вокруг разделения четырёх понятий:

```text
OWNERSHIP
    Кто отвечает за lifetime объекта?

USAGE
    Кто использует объект?

HANDLE
    Как безопасно идентифицировать объект?

RAW POINTER
    Как быстро работать с объектом после проверки?
```

Ответы:

```text
Asset
    owned by AssetManager

AssetInstance
    owned by InstanceManager

Character / GUI / Texture
    use resources, но не владеют ими

Handle
    идентифицирует ресурс и позволяет проверить его lifetime

Raw pointer
    используется локально после проверки
```

---

# 29. Главный архитектурный принцип

> Не нужно делать каждый pointer бессмертным.
>
> Нужно сделать ownership централизованным, lifetime предсказуемым, а ссылки проверяемыми.

Итоговая схема:

```text
Manager
    ↓
owns object

User
    ↓
stores Handle

Handle
    ↓
Manager::get()
    ↓
validation
    ↓
raw pointer
    ↓
fast C++ usage
```

При этом разные механизмы решают разные задачи:

```text
Ownership
    ↓
решает вопрос "кто удаляет?"

Lifetime
    ↓
решает вопрос "когда Manager существует?"

Handle
    ↓
решает вопрос "существует ли конкретный объект?"

Generation
    ↓
решает вопрос "не является ли Handle старым?"

Raw pointer
    ↓
решает вопрос "как удобно работать с объектом прямо сейчас?"
```

Такая архитектура позволяет сохранить простой стиль C++ с обычными указателями, не превращая систему в набор `owns_*` флагов, reference counters и взаимных деструкторов.

Та же архитектурная модель затем может быть перенесена на `Audio`, `Font`, `Animation` и другие типы ассетов.

---

# 30. Короткая формула системы

```text
AssetManager
    owns Asset

InstanceManager
    owns AssetInstance

Users
    use AssetInstance

Handles
    identify resources

Manager::get()
    validates Handle

Raw pointers
    используются после проверки

Scene lifetime
    гарантирует существование Managers

Generation
    защищает от stale handles
    и повторного использования slot
```

Итоговая философия системы:

```text
OWNERSHIP IS CENTRALIZED
USAGE IS NON-OWNING
IDENTIFICATION IS HANDLE-BASED
ACCESS IS POINTER-BASED
LIFETIME IS EXPLICIT
```

[Ожидаю команду "Продолжай"]