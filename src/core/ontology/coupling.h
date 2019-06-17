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
    
    void enable ()              { set_is_enabled (); };
    void enable (Process* data) { set_is_enabled (); _data = data; };
    void disable ()             { unset_is_enabled (); };
    bool is_enabled ()          { return is_flag_set (ENABLED_MASK, ENABLED_TRUE);  }


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

    void set_flag    (bit_mask MASK, bit_value VALUE) { _bitset = (_bitset & ~MASK) |  VALUE; }
    bool is_flag_set (bit_mask MASK, bit_value VALUE) { return    (_bitset &  MASK) == VALUE; }

    void set_is_enabled ()                {         set_flag    (ENABLED_MASK, ENABLED_TRUE);  }
    void unset_is_enabled ()              {         set_flag    (ENABLED_MASK, ENABLED_FALSE); }
  //bool is_enabled ()                    { return  is_flag_set (ENABLED_MASK, ENABLED_TRUE);  } // public

    void set_is_immediate_processing ()   {         set_flag    (IMMEDIATE_MASK, IMMEDIATE_TRUE);  }
    void unset_is_immediate_processing () {         set_flag    (IMMEDIATE_MASK, IMMEDIATE_FALSE); }
    bool is_immediate_processing ()       { return  is_flag_set (IMMEDIATE_MASK, IMMEDIATE_TRUE);  }

  //void unset_src_activation_flag ()     {         set_flag    (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_NONE); }
  //bool is_unset_src_activation_flag ()  { return  is_flag_set (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_NONE); }
  //bool is_set_src_activation_flag ()    { return !is_flag_set (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_NONE); }
    void src_request_activation ()        {         set_flag    (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_ACTIVATION); }
    bool is_src_activation_requested ()   { return  is_flag_set (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_ACTIVATION); }
    void src_request_deactivation ()      {         set_flag    (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_DEACTIVATION);  }
    bool is_src_deactivation_requested () { return  is_flag_set (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_DEACTIVATION);  }

  //void unset_dst_activation_flag ()     {         set_flag    (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_NONE); }
  //bool is_unset_dst_activation_flag ()  { return  is_flag_set (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_NONE); }
  //bool is_set_dst_activation_flag ()    { return !is_flag_set (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_NONE); }
    void dst_request_activation ()        {         set_flag    (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_ACTIVATION); }
    bool is_dst_activation_requested ()   { return  is_flag_set (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_ACTIVATION); }
    void dst_request_deactivation ()      {         set_flag    (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_DEACTIVATION);  }
    bool is_dst_deactivation_requested () { return  is_flag_set (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_DEACTIVATION);  }
    
    //bool __is_enabled;
    //bool __immediate_processing;
    //int m_src_flag, m_dst_flag;

    //void set_is_enabled () { __is_enabled = true; }
    //void unset_is_enabled () { __is_enabled = false; }
    //public: bool is_enabled() { return __is_enabled; }
    
    //void set_is_immediate_processing () { __immediate_processing = true; }
    //void unset_is_immediate_processing () { __immediate_processing = false; }
    //bool is_immediate_processing () { return __immediate_processing; }
    
  };

}
