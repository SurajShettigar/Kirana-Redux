$BUILD_ROOT_DIR="$PWD\build\release"
$ASSET_PATH="$PWD\assets\Kitchen_set\Kitchen_set.usd"

.venv/Scripts/activate

$env:PYTHONPATH+=";$BUILD_ROOT_DIR\lib\python"
$env:PATH+=";$BUILD_ROOT_DIR\lib;$BUILD_ROOT_DIR\bin"

Invoke-Expression "$BUILD_ROOT_DIR\bin\usdview  $ASSET_PATH"