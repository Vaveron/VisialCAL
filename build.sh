#!/bin/bash

# ==================== НАСТРОЙКИ ПУТЕЙ ====================
WX_DIR="$HOME/projects/VisialCAL/wxWidgets-3.2.10/windows-install"
WX_INCLUDE="$WX_DIR/include/wx-3.2"
WX_SETUP="$WX_DIR/lib/wx/include/x86_64-w64-mingw32-msw-unicode-static-3.2"
WX_LIB="$WX_DIR/lib"

# Компилятор и утилиты
CXX="x86_64-w64-mingw32-g++"
WINDRES="x86_64-w64-mingw32-windres"

# Директории проекта
SRC_DIR="src"
BUILD_DIR="build"
OBJ_DIR="$BUILD_DIR/obj"
BIN_DIR="$BUILD_DIR/bin"
LIBS_DIR="libs"
RES_DIR="resources"

# Создаём директории
mkdir -p "$OBJ_DIR" "$BIN_DIR"

# ==================== ФЛАГИ КОМПИЛЯЦИИ ====================
CXXFLAGS_COMMON="-I$WX_INCLUDE -I$WX_SETUP -I$SRC_DIR -I$LIBS_DIR"
CXXFLAGS_COMMON="$CXXFLAGS_COMMON -D__WXMSW__ -D__WXDEBUG__ -DwxUSE_UNICODE=1"
CXXFLAGS="$CXXFLAGS_COMMON -std=c++11 -O2 -ffunction-sections -fdata-sections"

RCFLAGS="-J rc -O coff -I$WX_INCLUDE"

LDFLAGS="-L$WX_LIB -static -mwindows -Wl,--gc-sections"
LDFLAGS="$LDFLAGS -l:libwx_mswu_core-3.2-x86_64-w64-mingw32.a"
LDFLAGS="$LDFLAGS -l:libwx_baseu-3.2-x86_64-w64-mingw32.a"
LDFLAGS="$LDFLAGS -l:libwxpng-3.2-x86_64-w64-mingw32.a"
LDFLAGS="$LDFLAGS -l:libwxjpeg-3.2-x86_64-w64-mingw32.a"
LDFLAGS="$LDFLAGS -l:libwxtiff-3.2-x86_64-w64-mingw32.a"
LDFLAGS="$LDFLAGS -l:libwxzlib-3.2-x86_64-w64-mingw32.a"
LDFLAGS="$LDFLAGS -l:libwxregexu-3.2-x86_64-w64-mingw32.a"
LDFLAGS="$LDFLAGS -l:libwxexpat-3.2-x86_64-w64-mingw32.a"
LDFLAGS="$LDFLAGS -lgdi32 -lole32 -loleaut32 -luuid -lcomctl32 -lcomdlg32"
LDFLAGS="$LDFLAGS -lshell32 -lshlwapi -lws2_32 -lwinmm -lversion -luxtheme"
LDFLAGS="$LDFLAGS -loleacc -lmsimg32 -lusp10 -lwinspool"

# ==================== ФУНКЦИИ ====================
error_exit() {
    echo "❌ Ошибка: $1"
    exit 1
}

compile_file() {
    local input="$1"
    local output="$2"
    echo "   Компиляция: $(basename "$input")"
    $CXX -c "$input" -o "$output" $CXXFLAGS
    if [ $? -ne 0 ]; then
        error_exit "Не удалось скомпилировать $input"
    fi
}

# ==================== ПОИСК ИСХОДНИКОВ ====================
echo "🔍 Поиск исходных файлов..."

# Собираем все .cpp файлы (кроме main.cpp и mathplot.cpp)
CPP_FILES=$(find "$SRC_DIR" -name "*.cpp" ! -name "main.cpp" 2>/dev/null)
MAIN_FILE="$SRC_DIR/main.cpp"
MATHPLOT_FILE="$LIBS_DIR/mathplot.cpp"

# Проверяем наличие main.cpp
[ ! -f "$MAIN_FILE" ] && error_exit "$MAIN_FILE не найден"

# Проверяем наличие mathplot
[ ! -f "$MATHPLOT_FILE" ] && error_exit "$MATHPLOT_FILE не найден"
[ ! -f "$LIBS_DIR/mathplot.h" ] && error_exit "mathplot.h не найден"

echo "✅ Найдено:"
echo "   - main.cpp"
for file in $CPP_FILES; do
    echo "   - $file"
done
echo "   - mathplot.cpp"

# ==================== КОМПИЛЯЦИЯ РЕСУРСОВ ====================
RESOURCE_OBJECT=""
if [ -f "$RES_DIR/resources.rc" ]; then
    echo ""
    echo "🎨 Компиляция ресурсов..."
    RESOURCE_OBJECT="$OBJ_DIR/resources.o"
    $WINDRES $RCFLAGS "$RES_DIR/resources.rc" -o "$RESOURCE_OBJECT"
    [ $? -ne 0 ] && error_exit "Компиляция ресурсов"
    echo "✅ Ресурсы скомпилированы"
fi

# ==================== КОМПИЛЯЦИЯ ФАЙЛОВ ====================
echo ""
echo "🔧 Компиляция исходников..."
OBJECTS=""

# Компиляция mathplot.cpp
MATHPLOT_OBJECT="$OBJ_DIR/mathplot.o"
compile_file "$MATHPLOT_FILE" "$MATHPLOT_OBJECT"
OBJECTS="$OBJECTS $MATHPLOT_OBJECT"

# Компиляция всех остальных .cpp файлов (кроме main.cpp)
for file in $CPP_FILES; do
    rel_path="${file#$SRC_DIR/}"  # Убираем src/ из пути
    obj_name="${rel_path//\//_}"   # Заменяем / на _
    obj_name="${obj_name%.cpp}.o"
    obj_file="$OBJ_DIR/$obj_name"
    
    compile_file "$file" "$obj_file"
    OBJECTS="$OBJECTS $obj_file"
done

# Компиляция main.cpp
MAIN_OBJECT="$OBJ_DIR/main.o"
compile_file "$MAIN_FILE" "$MAIN_OBJECT"
OBJECTS="$OBJECTS $MAIN_OBJECT"

# Добавляем ресурсы
[ -n "$RESOURCE_OBJECT" ] && OBJECTS="$OBJECTS $RESOURCE_OBJECT"

# ==================== ЛИНКОВКА ====================
echo ""
echo "🔗 Линковка..."
OUTPUT="$BIN_DIR/app.exe"

$CXX $OBJECTS -o "$OUTPUT" $LDFLAGS
[ $? -ne 0 ] && error_exit "Линковка"

echo "✅ Готово: $OUTPUT"

# ==================== ОЧИСТКА ====================
# Опционально: оставляем .o файлы для ускорения пересборки
# Для очистки всех временных файлов раскомментируйте:
# rm -rf "$OBJ_DIR"/*.o

echo ""
echo "📊 Статистика:"
echo "   Размер: $(du -h "$OUTPUT" | cut -f1)"
echo "   Файл: $(pwd)/$OUTPUT"
echo ""
echo "💡 Запуск в Windows: \\\\wsl.localhost\\Ubuntu\\$(pwd)/$OUTPUT"