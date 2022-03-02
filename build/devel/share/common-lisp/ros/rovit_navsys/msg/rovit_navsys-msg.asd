
(cl:in-package :asdf)

(defsystem "rovit_navsys-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "debug" :depends-on ("_package_debug"))
    (:file "_package_debug" :depends-on ("_package"))
  ))