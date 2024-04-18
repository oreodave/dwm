;;; Directory Local Variables         -*- no-byte-compile: t; -*-
;;; For more information see (info "(emacs) Directory Variables")

((c-mode . ((clang-format-mode . nil)
            (flycheck-mode . nil)
            (eval . (remove-hook 'c-mode-hook #'clang-format-mode)))))
