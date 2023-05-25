# Qt Icon Theme Manager

<img src="https://github.com/konero/iconmanager/blob/master/img/iconManagerApp.gif">

Handled by `IconManager` class, which manages icon themes, a light theme and dark theme. The class reads icons from a specified path and generates two versions of each icon (light and dark) by `recoloring` the original images. It stores these icon versions in two separate `QMaps` (`iconMapLight` and `iconMapDark`), mapping icon names to `QIcon` objects.

The `currentIconMap` pointer points to the currently active icon map. This enables us to retrieve the right version of the icon based on the current theme by calling `IconManager::getIcon()`.

The `IconManager::switchTheme(bool)` is a function that switches the current icon theme. This is done by updating the `currentIconMap` pointer to point to the corresponding icon map (`iconMapDark` for dark theme, `iconMapLight` for light theme). When this function is called, it emits a `themeChanged()` signal.

I created a custom widget `TToolButton` that inherits from `QToolButton`. This widget takes an IconManager instance and an icon name as params. When a `themeChanged()` signal is emitted, `TToolButton` responds by calling its `updateIcon()` slot function, which updates its icon to the current theme's version using the `IconManager::getIcon()` function.

Finally, we connected the `themeChanged()` signal to the `updateIcon()` slot function for all instances of `TToolButton`. This ensures that every icon in the application is updated whenever the theme is changed.
