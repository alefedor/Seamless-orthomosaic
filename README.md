# Seamless-orthomosaic
## Сборка

Собирается с помощью цели orthomosaic в CMake

## Структура для запуска программы

path/orthomosaic -- собранный файл

path/images.csv -- файл с информацией об ортофотографиях

path/orthophotos/ -- ортофотографии

## Формат images.csv

Первая строчка:
name,left,top,center_x,center_y,width,height

Далее идут описание ортофотографий в формате соответствующем первой строчке.

left, top -- левая и верхняя границы ортофотографии в ортофотоплане.

width, height -- ширина и высота в пикселях.

## Запуск
./orthomosaic left top width height 

или

./orthomosaic left top width height orthophoto1 orthophoto2

В зависимости от того, хотим мы объединить две определенные ортофотографии или все, пересекающиеся с областью интереса.
left top width height -- параметры области интереса
