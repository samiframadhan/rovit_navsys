// Auto-generated. Do not edit!

// (in-package rovit_navsys.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class debug {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.data = null;
      this.leftSpeed = null;
      this.rightSpeed = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('data')) {
        this.data = initObj.data
      }
      else {
        this.data = [];
      }
      if (initObj.hasOwnProperty('leftSpeed')) {
        this.leftSpeed = initObj.leftSpeed
      }
      else {
        this.leftSpeed = 0.0;
      }
      if (initObj.hasOwnProperty('rightSpeed')) {
        this.rightSpeed = initObj.rightSpeed
      }
      else {
        this.rightSpeed = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type debug
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [data]
    bufferOffset = _arraySerializer.float32(obj.data, buffer, bufferOffset, null);
    // Serialize message field [leftSpeed]
    bufferOffset = _serializer.float32(obj.leftSpeed, buffer, bufferOffset);
    // Serialize message field [rightSpeed]
    bufferOffset = _serializer.float32(obj.rightSpeed, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type debug
    let len;
    let data = new debug(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [data]
    data.data = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [leftSpeed]
    data.leftSpeed = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [rightSpeed]
    data.rightSpeed = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 4 * object.data.length;
    return length + 12;
  }

  static datatype() {
    // Returns string type for a message object
    return 'rovit_navsys/debug';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'f1b41bdf0f62fad14787c3f9671d9cfa';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new debug(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.data !== undefined) {
      resolved.data = msg.data;
    }
    else {
      resolved.data = []
    }

    if (msg.leftSpeed !== undefined) {
      resolved.leftSpeed = msg.leftSpeed;
    }
    else {
      resolved.leftSpeed = 0.0
    }

    if (msg.rightSpeed !== undefined) {
      resolved.rightSpeed = msg.rightSpeed;
    }
    else {
      resolved.rightSpeed = 0.0
    }

    return resolved;
    }
};

module.exports = debug;
