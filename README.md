# rggobi

## Building from source

For a fresh checkout, you'll need to run

```
aclocal
autoconf
```

## ggobi/rggobi on OS X

Currently, the easiest way to get rggobi on OS X is to get Gtk2 from homebrew, then install ggobi [from source](https://github.com/ggobi/ggobi):

```
# Install homebrew (if you don't already have it)
brew install gtk+
brew install libxml2

# Set up PKG_CONFIG_PATH so rggobi can find all the pieces
PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:/opt/X11/lib/pkgconfig:/usr/local/opt/libxml2/lib/pkgconfig

R
install.packages("rggobi", type = "source")
```
