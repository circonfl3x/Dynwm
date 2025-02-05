# Dynwm
A small utility aimed at tiling window managers that enables some form of dynamic wallpaper functionality (still trash, messy code, some memory leaks etc)

## Usage
`./Dynwm {config_file}`
For now it only supports Dynamic wallpapers, but in the future I'll look to add Light/dark theme support (QT/GTK?) and maybe a random wallpaper selector

## Config
Check the example config file provided for a template.

A `:` goes before every command specifier.

For example `:type "Dynamic"` specifies the type is dynamic (the argument is always in quotes)

For the command for the wallpaper daemon, write the command as you would and then `${}` in place of the filepath. So:

`:cmd "blah blah ${} blah blah"`

Example with swww:

`:cmd "swww img ${} ---transition-type=fade"`

Example with swaybg:

`:cmd "swaybg -i ${}"`

For the wallpapers' section, the beginning is marked by a `:begin` and the end by an `:end`, so:

```
:begin

....
....
....

:end
```

All wallpapers in Dynamic mode will be specified in here.

To specify a wallpaper, it is in the format `time=filepath`. (No double quotes around the filepath)

An example:

`05:30=dir/name.jpg` or `5:30=dir/name.jpg`

Another example:

`09:05=dir/name.jpg` or `9:5=dir/name.jpg` (second one is a war crime)

In both the first case is better because it is well... Aesthetically more pleasing 😂

If it is directly on the hour, there is no need to specify the `00` or `0` so it can be written as:

`7=dir/name.jpg` if it is 07:00

That's about all in terms of config for now.
There is the other `:light "dir/name.jpg"` and the `:dark "dir/name.jpg"` for the `:type "Type"` but it isn't implemented for now
