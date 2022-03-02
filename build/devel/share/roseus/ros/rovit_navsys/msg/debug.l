;; Auto-generated. Do not edit!


(when (boundp 'rovit_navsys::debug)
  (if (not (find-package "ROVIT_NAVSYS"))
    (make-package "ROVIT_NAVSYS"))
  (shadow 'debug (find-package "ROVIT_NAVSYS")))
(unless (find-package "ROVIT_NAVSYS::DEBUG")
  (make-package "ROVIT_NAVSYS::DEBUG"))

(in-package "ROS")
;;//! \htmlinclude debug.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass rovit_navsys::debug
  :super ros::object
  :slots (_header _data _leftSpeed _rightSpeed ))

(defmethod rovit_navsys::debug
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:data __data) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:leftSpeed __leftSpeed) 0.0)
    ((:rightSpeed __rightSpeed) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _data __data)
   (setq _leftSpeed (float __leftSpeed))
   (setq _rightSpeed (float __rightSpeed))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:leftSpeed
   (&optional __leftSpeed)
   (if __leftSpeed (setq _leftSpeed __leftSpeed)) _leftSpeed)
  (:rightSpeed
   (&optional __rightSpeed)
   (if __rightSpeed (setq _rightSpeed __rightSpeed)) _rightSpeed)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float32[] _data
    (* 4    (length _data)) 4
    ;; float32 _leftSpeed
    4
    ;; float32 _rightSpeed
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float32[] _data
     (write-long (length _data) s)
     (dotimes (i (length _data))
       (sys::poke (elt _data i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32 _leftSpeed
       (sys::poke _leftSpeed (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _rightSpeed
       (sys::poke _rightSpeed (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float32[] _data
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _data (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _data i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; float32 _leftSpeed
     (setq _leftSpeed (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _rightSpeed
     (setq _rightSpeed (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get rovit_navsys::debug :md5sum-) "f1b41bdf0f62fad14787c3f9671d9cfa")
(setf (get rovit_navsys::debug :datatype-) "rovit_navsys/debug")
(setf (get rovit_navsys::debug :definition-)
      "Header header
float32[] data
float32 leftSpeed
float32 rightSpeed
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

")



(provide :rovit_navsys/debug "f1b41bdf0f62fad14787c3f9671d9cfa")


