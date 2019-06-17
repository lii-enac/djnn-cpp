/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2018)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *      Stephane Conversy <stephane.conversy@enac.fr>
 *
 */

#pragma once

namespace djnn {

  class Process;

  class Coupling {
  public:
    Coupling (Process* src, int src_flag, Process* dst, int dst_flag, bool immediate_processing = false);
    Coupling (Process* src, int src_flag, Process* dst, int dst_flag, Process* data, bool immediate_processing = false);
    virtual ~Coupling();
    void set_data (Process* data) { _data = data; }
    
    void propagate_activation ();
    void propagate_deactivation ();

    // process
    Process* get_src () { return _src; }
    Process* get_dst () { return _dst; }
    
    void enable ()              { set_enabled (true); };
    void enable (Process* data) { set_enabled (true); _data = data; };
    void disable ()             { set_enabled (false); };
    bool is_enabled ()          { return get_enabled (); }

  private:
    void init_coupling (Process* src, int src_flag, Process* dst, int dst_flag);
    void process_immediately ();

    Process *_src, *_dst, *_data;

    unsigned int _bitset;
    
    enum bit_shift {
        ENABLED_SHIFT             = 0b000 ,
        IMMEDIATE_SHIFT           = 0b001 ,
        SRC_ACTIVATION_FLAG_SHIFT = 0b010 ,
        DST_ACTIVATION_FLAG_SHIFT = 0b100 ,
    };

    enum bit_mask {
        ENABLED_MASK              = 0b1  << ENABLED_SHIFT ,
        IMMEDIATE_MASK            = 0b1  << IMMEDIATE_SHIFT ,
        SRC_ACTIVATION_FLAG_MASK  = 0b11 << SRC_ACTIVATION_FLAG_SHIFT ,
        DST_ACTIVATION_FLAG_MASK  = 0b11 << DST_ACTIVATION_FLAG_SHIFT
    };

    enum bit_value {
        ENABLED_FALSE                     = 0b0  << ENABLED_SHIFT ,
        ENABLED_TRUE                      = 0b1  << ENABLED_SHIFT ,

        IMMEDIATE_FALSE                   = 0b0  << IMMEDIATE_SHIFT ,
        IMMEDIATE_TRUE                    = 0b1  << IMMEDIATE_SHIFT ,

        SRC_ACTIVATION_FLAG_NONE          = 0b00 << SRC_ACTIVATION_FLAG_SHIFT ,
        SRC_ACTIVATION_FLAG_ACTIVATION    = 0b01 << SRC_ACTIVATION_FLAG_SHIFT ,
        SRC_ACTIVATION_FLAG_DEACTIVATION  = 0b10 << SRC_ACTIVATION_FLAG_SHIFT ,

        DST_ACTIVATION_FLAG_NONE          = 0b00 << DST_ACTIVATION_FLAG_SHIFT ,
        DST_ACTIVATION_FLAG_ACTIVATION    = 0b01 << DST_ACTIVATION_FLAG_SHIFT ,
        DST_ACTIVATION_FLAG_DEACTIVATION  = 0b10 << DST_ACTIVATION_FLAG_SHIFT ,
    };

    void set_flag    (bit_mask MASK, bit_value VALUE)             { _bitset = (_bitset & ~MASK) |  VALUE; }
    void set_flag    (bit_mask MASK, bit_shift SHIFT, int VALUE)  { _bitset = (_bitset & ~MASK) | (VALUE << SHIFT); }
    bool is_flag_set (bit_mask MASK, bit_value VALUE)             { return    (_bitset &  MASK) == VALUE; }
    int  get_flag    (bit_mask MASK, bit_shift SHIFT)             { return    (_bitset &  MASK) >> SHIFT; }

    bool get_enabled ()                    { return get_flag    (ENABLED_MASK, ENABLED_SHIFT); }
    void set_enabled (bool VALUE)          {        set_flag    (ENABLED_MASK, ENABLED_SHIFT, VALUE); }

    bool get_immediate_processing ()       { return get_flag    (IMMEDIATE_MASK, IMMEDIATE_SHIFT); }
    void set_immediate_processing (bool VALUE) {    set_flag    (IMMEDIATE_MASK, IMMEDIATE_SHIFT, VALUE); }

    int  get_src_activation_flag ()        { return get_flag    (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_SHIFT); }
    void set_src_activation_flag (int VALUE) {      set_flag    (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_SHIFT, VALUE); }

    int  get_dst_activation_flag ()       { return  get_flag    (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_SHIFT); }
    void set_dst_activation_flag (int VALUE) {      set_flag    (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_SHIFT, VALUE); }
    
  };

}
