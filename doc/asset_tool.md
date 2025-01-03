# Утилита asset_tool

Утилита `asset_tool` нужна для упаковки разных ресурсов (текстуры, шейдеры и др.) в формат, понятный движку.
Для удобства разработки и читаемости предусмотрено чтение и запись в формате JSON. Для готовых приложений,
где важна скорость чтения и разбора, предусмотрен формат flatbuffers. С помощью данной утилиты можно
конвертировать ресурсы между этими форматами.

## Использование утилиты

Утилита `asset_tool` должна вызываться с определёнными параметрами, после которых должны следовать
имена файлов ресурсов (кроме режима распаковки пакета, см. далее).

Параметры утилиты:

* `--help`, `-h` - отображение справки и выход.
* `--base-dir DIR`, `-b DIR` - базовая директория для чтения ресурсов из отдельных файлов.
Требуется для форматов, в которых данные ресурса хранятся отдельными файлами от файла описания
(таким форматом является JSON, см. далее). По умолчанию используется текущая директория.
* `--out-dir DIR`, `-o DIR` - директория, в которую будут записаны результаты выполнения программы.
По умолчанию используется текущая директория. Расширения выходных файлов подставляются в зависимости
от выходного формата, указанного в параметре `--dst-format`.
* `--src-format FORMAT`, `-s FORMAT` - ожидаемый формат входных данных. Список форматов данных см. далее.
По умолчанию используется `json`.
* `--dst-format FORMAT`, `-d FORMAT` - формат выходных данных. Список форматов данных см. далее.
По умолчанию используется `flatbuffers`.
* `--pack PACKAGE_NAME`, `-p PACKAGE_NAME` - имя выходного пакета ресурсов (относительно выходной директории,
указанной в параметре `--out-dir`). Использование этого параметра переводит утилиту в режим создания пакета.
Это означает что все файлы ресурсов, переданные утилите, будут объединены в один пакет (в формате, указанном
в параметре `--out-dir`). Этот параметр взаимно исключает параметр `--unpack` и не должен использоваться
вместе с ним. Расширение файла будет автоматически подставлено в зависимости от выходного формата.
* `--unpack PACKAGE_NAME`, `-u PACKAGE_NAME` - имя входного пакета ресурсов. Использование этого параметра
переводит утилиту в режим распаковки пакета. В этом режиме из пакета будут извлечены все ресурсы и помещены
в директорию, указанную в параметре `--out-dir`. В этом режиме не должен быть указан список файлов. Этот
параметр взаимно исключает параметр `--pack` и не должен использоваться вместе с ним.
* `--quiet` - отключить информационный вывод утилиты (вывод ошибок в поток вывода ошибок остаётся). По
умолчанию информационный вывод присутствует.

Форматы данных:

* `json` - JSON, см. страницу "Сериализация ресурсов в формате JSON". Расширение файлов ресурсов и 
пакетов - `.json`. Формат доступен, если при сборке была включена опция `NARENGINE_TOOLS_JSON` в CMake.
* `flatbuffers` - flatbuffers, для описания структур см. директорию `schemas` в исходном коде.
Расширение файлов ресурсов - `.narasset`, файлов пакетов - `.nrs`. Формат доступен, если при сборке
была включена опция `NARENGINE_TOOLS_FLATBUFFERS` в CMake.

### Примеры

Вызов утилиты для конвертации текстуры и шейдера из формата JSON в формат flatbuffers:
```
asset_tool --base-dir resource_data --out-dir ouput_data --src-format json --dst-format flatbuffers texture.json shader.json
```
В результате будут созданы файлы `output_data/texture.narasset` и `output_data/shader.narasset`.

Вызов утилиты для упаковки текстуры и шейдера в пакет `package` (директория вывода - текущая - по умолчанию):
```
asset_tool --pack package --src-format flatbuffers --dst-format flatbuffers texture.narasset shader.narasset
```
В результате в текущей директории будет создан файл `package.nrs`.

Вызов утилиты для распаковки пакета `package` в формате flatbuffers на ресурсы в формате JSON:
```
asset_tool --unpack package.nrs --out-dir ouput_data --src-format flatbuffers --dst-format json
```
Если в пакете было два ресурса, как в предыдущем примере, то в результате получатся файлы
`output_data/texture.json` и `output_data/shader.json` и отдельные файлы с данными в директории
`output_data`, необходимые в формате JSON (см. далее).

## Библиотека 16nar_tools

Утилита `asset_tool` использует библиотеку `16nar_tools`. Если нужно прочитать/записать ресурсы в
программе, то можно использовать эту библиотеку.

В библиотеке есть интерфейсы `IAssetReader` и `IAssetWriter`, а также классы, реализующие их:
`JsonAssetReader`, `JsonAssetWriter`, `FlatBuffersAssetReader`, `FlatBuffersAssetWriter`.
Для общего представления ресурсов, не зависящего от формата, используется структура `ResourceData`,
а для представления пакета ресурсов - структура `PackageData`.
Все классы объявлены в пространстве имён `_16nar::tools`.

## Информация о формате JSON

Описание формата см. на странице "Сериализация ресурсов в формате JSON".

В формате JSON сохраняются только параметры загрузки ресурсов. Сами данные хранятся в отдельных файлах,
пути к которым указаны в JSON-файле. Таким образом, для представления ресурса в формате JSON требуется
несколько файлов. Пути к файлам данных в JSON-файле могут быть абсолютные или относительные (рекомендуется
использовать относительные пути для переносимости).

Относительные пути в формате JSON будут подставлены с использованием базовой директории, указанной
в одном из параметров `asset_tool`:
* `--base-dir <DIR>`, `-b <DIR>` - директория, подставляемая при чтении ресурсов.
* `--out-dir <DIR>`, `-o <DIR>` - директория, подставляемая при записи ресурсов.

При использовании библиотеки данные директории указываются в параметрах консторуторов у классов
`JsonAssetReader` и `JsonAssetWriter`, соответственно.

В формате flatbuffers все данные и параметры загрузки объединены в одном файле ресурса, поэтому для
этого формата данные параметры не используются.

### Пример

Допустим, есть файл ресурса (текстуры) `test_texture.json`, в поле `file` которго хранится
относительный путь `test_texture.png`. Файл `test_texture.png` находится в директории
`resource_data`, а сам файл ресурса `test_texture.json` может находится в другой директории.

При вызове команды:
```
asset_tool --base-dir resource_data --out-dir ouput_data --src-format json --dst-format flatbuffers test_texture.json
```
указанный файл `test_texture.json` будет сконвертирован из формата JSON в формат flatbuffers.
Результат будет записан в файл `output_data/test_texture.narasset`. При чтении данных будет
разобрано поле `file` в JSON, и оттуда будет взят файл `test_texture.png`. Затем для получения
полного пути будет взята указанная директория `resource_data`, и, в результате, будут считаны
данные текстуры из файла `resource_data/test_texture.png`.

Таким образом, можно указывать директорию, не меняя пути в самом JSON-файле.

То же справедливо и для обратной конвертации в формат JSON. Например, при вызове команды:
```
asset_tool --out-dir ouput_data --src-format flatbuffers --dst-format json test_texture.narasset
```
будут созданы файлы `ouput_data/test_texture_texture.bin` и `ouput_data/test_texture.json`
(имя файла зависит от имени ресурса, например, в формате JSON оно хранится в поле `name`).
При этом, в поле `file` будет указан относительный путь `test_texture_texture.bin` без
директории `ouput_data`.
