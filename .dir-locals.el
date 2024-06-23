;;; Directory Local Variables         -*- no-byte-compile: t; -*-
;;; For more information see (info "(emacs) Directory Variables")

((c-mode . ((indent-tabs-mode . t)
            (flycheck-disabled-checkers c/c++-clang c/c++-gcc)))
 (nil    . ((compile-command  . "make && sudo make install && killall dwm"))))
