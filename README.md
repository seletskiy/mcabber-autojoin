mcabber-autojoin
================

Module provides autojoin functionality for stupid servers which does not
support bookmarks.

## Installation

### Arch Linux

```
git checkout pkgbuild
makepkg
pacman -U *.tar.xz
```

### Other

```
make
cp libautojoin.so /usr/lib/mcabber/
```

## Usage

Module expects option called `autojoin`, which is comma-separated list of
conferences to join.

```
set autojoin = general@blah.qux.example.com,random@bar.tor.example.com
```

**Trick**: if `@<host>` part is ommited, then hostname from previous conference
will be reused, like this:

```
set autojoin = a@lala.example.com,b,c,d

# same, as:

set autojoin = a@lala.example.com,b@lala.example.com,c@lala.example.com,...
```

