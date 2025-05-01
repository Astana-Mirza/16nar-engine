# Сериализация ресурсов в формате JSON

Для удобства разработки можно хранить ресурсы в формате JSON. При этом сами данные будут сохранены
в отдельных файлах, потому что их неудобно хранить в том же JSON-файле. Таким образом, в JSON
хранятся параметры загрузки, необходимые для движка, и имя файла с бинарными данными ресурса.

Далее рассмотрены примеры хранения в формате JSON для разных типов ресурсов.

## Представление перечислений

Для представления имеющихся перечислений (enum) в JSON используются следуюшие значения в виде строк:

| Перечисление     | Значения                                                                                                                                 |
|------------------|------------------------------------------------------------------------------------------------------------------------------------------|
| ResourceType     | `texture`, `shader`, `vertex_buffer`, `cubemap`, `data_schema`                                                                                          |
| TextureWrap      | `repeat`, `mirrored_repeat`, `clamp_to_edge`, `clamp_to_border`                                                                          |
| TextureFilter    | `nearest`, `linear`, `nearest_mipmap_nearest`, `nearest_mipmap_linear`, `linear_mipmap_nearest`, `linear_mipmap_linear`                  |
| BufferDataFormat | `rgb`, `rgba`, `srgb`, `srgba`                                                                                                           |
| DataType         | `byte`, `float`                                                                                                                          |
| ShaderType       | `vertex`, `fragment`, `geometry`                                                                                                         |
| BufferType       | `stream_draw`, `stream_read`, `stream_copy`, `static_draw`, `static_read`, `static_copy`, `dynamic_draw`, `dynamic_read`, `dynamic_copy` |

## Представление ресурсов

Описание общих полей:
- `type` - тип ресурса, тип ResourceType.
- `name` - имя в представлении движка (в классе `PackageManager`).
- `file` - имя файла, в котором содержатся бинарные данные ресурса.
- `wrap_<N>` - способ обработки координат, выходящих за пределы текстуры, по оси `<N>`, тип TextureWrap.
- `format` - формат интерпретации данных из буфера, BufferDataFormat.
- `min_filter` - тип фильтра при уменьшении текстуры при масштабировании, тип TextureFilter.
- `mag_filter` - тип фильтра при увеличении текстуры при масштабировании, тип TextureFilter.
- `data_type` - тип данных буфера, тип DataType.

### Texture

Пример:

```
{
     "type": "texture",
     "name": "our_texture",
     "format": "rgba",
     "min_filter": "nearest",
     "mag_filter": "nearest",
     "wrap_x": "repeat",
     "wrap_y": "repeat",
     "data_type": "byte",
     "samples": 0,
     "raw": false,
     "size": [ 256, 256 ],
     "border_color": [ 0.8, 0.24, 0.55, 1.0 ],
     "file": "our_texture.png"
}
```

Описание полей:
- `samples` - количество сэмплов для встроенного сглаживания. Если равно 0, то не используется.
Обычно не используется, потому что сглаживание реализуется шейдерами.
- `raw` - хранятся ли в файле `file` готовые к использованию бинарные данные (в этом случае значение true)
или требуется дополнительная обработка (значение false).
- `size` - двухмерный целочисленный вектор с размерами текстуры (x;y).
- `border_color` - четырёхмерный целочисленный вектор с цветом рамки текстуры, используется при
соответствующем TextureWrap.

### Shader

Пример:

```
{
     "type": "shader",
     "name": "our_shader",
     "shaders":[
          {
               "entrypoint": "main",
               "type": "vertex",
               "file": "our_shader_vertex.glsl",
               "from_source": true
          },
          {
               "entrypoint": "main",
               "type": "fragment",
               "file": "our_shader_fragment.glsl",
               "from_source": true
          }
     ]
}
```

Описание полей:
- `shaders` - массив с описанием объектов шейдеров.
- `entrypoint` - точка входа в шейдер (имя функции).
- `type` - в объекте из массива `shaders` - тип шейдера, тип ShaderType.
- `from_source` - нужно ли компилировать шейдер из файла `file` (в этом случае значение true) или
нужно загрузить в неизменном виде (значение false). Если этот флаг выставлен, то во время загрузки
исходного кода шейдера из файла в конец будет добавлен нулевой байт. Это нужно, чтобы передать
графическому API строку, оканчивающуюся нулём. При сохранении файла нулевой байт убирается и не
попадает в итоговый файл на диске. Таким образом, можно переиспользовать этот файл для создания
шейдеров.

### VertexBuffer

Пример:

```
{
     "type": "vertex_buffer",
     "name": "our_vertex_buffer",
     "buffer": {
          "file": "our_vbo.bin",
          "type": "static_draw"
     },
     "index_buffer": {
          "file": "our_ebo.bin",
          "type": "static_draw"
     },
     "attributes": [
          {
               "data_type": "float",
               "size": 2,
               "normalized": false
          },
          {
               "data_type": "float",
               "size": 2,
               "normalized": false
          }
     ]
}
```

Описание полей:
- `buffer` - буфер с данными для отрисовки.
- `index_buffer` - буфер с индексами для отрисовки.
- `type` - в описании буферов `buffer` и `index_buffer` - тип буфера данных, тип BufferType.
- `size` - в описании атрибутов в массиве `attributes` - количество измерений в атрибуте.
- `normalized` - нужно ли нормализовать данные атрибута при загрузке.

### Cubemap

Пример:

```
{
     "type": "cubemap",
     "name": "our_cubemap",
     "format": "rgba",
     "min_filter": "nearest",
     "mag_filter": "nearest",
     "wrap_x": "repeat",
     "wrap_y": "repeat",
     "wrap_z": "repeat",
     "data_type": "byte",
     "raw": false,
     "size": [ 256, 256 ],
     "border_color": [ 0.8, 0.24, 0.55, 1.0 ],
     "files": [ "our_cm1.png", "our_cm2.png", "our_cm3.png", "our_cm4.png", "our_cm5.png", "our_cm6.png" ]
}
```

Описание полей:
- `files` - массив из 6 имён файлов с текстурами кубической карты.

### DataSchema

Схема данных содержит метаинформацию о свойствах, см. Обобщённый формат хранения свойств.

Пример:

```
{
     "type": "data_schema",
     "name": "our_data_schema",
     "items":[
          {
               "type": "string",
               "name": "our_string",
               "mandatory": true
          },
          {
               "type": "int32",
               "name": "our_int32",
               "mandatory": true
          },
          {
               "type": "vec2f",
               "name": "our_vec2f",
               "mandatory": false
          },
          {
               "type": "bool",
               "name": "our_bool",
               "mandatory": true
          }
     ],
     "default_vals":{
          "our_string": "some_value",
          "our_int32": 777
     }
}
```

Описание полей:
- `items` - массив с объектами, каждый из которых представляет информацию об одном свойстве.
В поле `type` каждого объекта содержится тип свойства в виде строки (типы см. Обобщённый формат хранения свойств),
в поле `name` содержится имя свойства, а в поле `mandatory` - признак обязательности наличия этого свойства в хранилище.
- `default_vals` - объект, представляющий хранилище свойств в виде пар "ключ-значение". Данное хранилище содержит
значения полей по умолчанию, ассоциированные с именами полей.

## Представление пакета ресурсов

Пакеты ресурсов содержат информацию о нескольких ресурсах. В поле `resources` хранится массив объектов ресурсов,
описанных выше.

В поле `chunk_size` содержится размер одного участка файла в байтах. В бинарных форматах пакетов, в которых
ресурсы упаковываются в один файл, пакет может быть загружен в память не полностью. Вместо этого могут быть
загружены только нужные участки. Для этого указывается размер участка. При загрузке нужные участки будут
выбраны на основе размеров ресурсов и того факта, что ресурсы идут подряд. Первый участок начинается в самом
начале файла и, таким образом, содержит заголовок.

Пример:

```
{
     "resources": [
          {
               "type": "texture",
               "name": "our_texture",
               "format": "rgba",
               "min_filter": "nearest",
               "mag_filter": "nearest",
               "wrap_x": "repeat",
               "wrap_y": "repeat",
               "data_type": "byte",
               "samples": 0,
               "raw": false,
               "size": [ 256, 256 ],
               "border_color": [ 0.8, 0.24, 0.55, 1.0 ],
               "file": "our_texture.png"
          },
          {
               "type": "shader",
               "name": "our_shader",
               "shaders":[
                    {
                         "entrypoint": "main",
                         "type": "vertex",
                         "file": "our_shader_vertex.glsl",
                         "from_source": true
                    },
                    {
                         "entrypoint": "main",
                         "type": "fragment",
                         "file": "our_shader_fragment.glsl",
                         "from_source": true
                    }
               ]
          }
     ],
     "chunk_size": 10240
}
```

В данном пакете содержится два ресурса: текстура и шейдер. Размер одного участка составляет 10 КиБ.
