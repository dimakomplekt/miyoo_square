# Добавляем MinGW в PATH
$env:PATH += ";C:\mingw-w64\mingw64\bin"

# Если папки build нет — создаём и конфигурируем проект
if (-not (Test-Path "build")) {
    Write-Host "Папки build нет — создаю и настраиваю проект..."
    cmake -S . -B build -G "MinGW Makefiles"
} else {
    Write-Host "Папка build уже есть — пропускаю конфигурацию."
}

# Собираем проект
cmake --build build --config Release

# Если сборка прошла успешно — запускаем
if ($LASTEXITCODE -eq 0) {
    Write-Host "Запускаю программу..."
    .\build\miyoo_square.exe
} else {
    Write-Host "Ошибка сборки. Исправь проблемы и попробуй снова."
}
