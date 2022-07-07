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

#include "process.h"

namespace djnn {

  class Coupling {
  public:
    Coupling (CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag, bool immediate_propagation = false);
    Coupling (); // needed for pointer-less zombie initialization in Binding, Assignment, Connector
    ~Coupling();
    
    void propagate_activation ();
    void propagate_deactivation ();

    CoreProcess* get_src ()  { return _src; }
    CoreProcess* get_dst ()  { return _dst; }
    void set_src (CoreProcess* src);
    void set_dst (CoreProcess* dst);

    void enable ()              { set_is_enabled (true); }
    void disable ()             { set_is_enabled (false); }
    bool is_enabled () const    { return get_is_enabled (); }
    bool is_immediate () const  { return get_immediate_propagation (); }
    bool is_effective () const  { return _src != nullptr /*&& _dst != nullptr*/; }

    void about_to_delete_src () { _src = nullptr; }

    void init (CoreProcess* src, activation_flag_e src_flag, CoreProcess* dst, activation_flag_e dst_flag, bool immediate_propagation = false);
    void uninit ();

  protected:
    void propagate_immediately ();

    CoreProcess *_src, *_dst;
    unsigned int _bitset;
    
    enum bit_shift {
        ENABLED_SHIFT             = 0 ,
        IMMEDIATE_SHIFT           = 1 ,
        SRC_ACTIVATION_FLAG_SHIFT = 2 , // FIXME: NONE is never used? if so, 1 bit is enough
        DST_ACTIVATION_FLAG_SHIFT = 4 , // FIXME: NONE is never used? if so, 1 bit is enough
        HAS_GRAPH_EDGE_SHIFT      = 6
    };

    enum bit_mask {
        ENABLED_MASK              = 0b1  << ENABLED_SHIFT , // FIXME, shouldn't it be 0b0?!
        IMMEDIATE_MASK            = 0b1  << IMMEDIATE_SHIFT ,
        SRC_ACTIVATION_FLAG_MASK  = 0b11 << SRC_ACTIVATION_FLAG_SHIFT ,
        DST_ACTIVATION_FLAG_MASK  = 0b11 << DST_ACTIVATION_FLAG_SHIFT ,
        HAS_GRAPH_EDGE_MASK       = 0b1  << HAS_GRAPH_EDGE_SHIFT
    };

    void set_bitset (bit_mask MASK, bit_shift SHIFT, unsigned int VALUE)  { _bitset = (_bitset & ~MASK) | (VALUE << SHIFT); }
    int  get_bitset (bit_mask MASK, bit_shift SHIFT) const       { return ( _bitset &  MASK) >> SHIFT; }

    bool get_is_enabled () const            { return get_bitset (ENABLED_MASK, ENABLED_SHIFT); }
    void set_is_enabled (bool VALUE)        {        set_bitset (ENABLED_MASK, ENABLED_SHIFT, VALUE); }

    bool get_has_edge () const              { return get_bitset (HAS_GRAPH_EDGE_MASK, HAS_GRAPH_EDGE_SHIFT); }
    void set_has_edge (bool VALUE)          {        set_bitset (HAS_GRAPH_EDGE_MASK, HAS_GRAPH_EDGE_SHIFT, VALUE); }

    bool get_immediate_propagation () const { return get_bitset (IMMEDIATE_MASK, IMMEDIATE_SHIFT); }
    void set_immediate_propagation (bool VALUE) {    set_bitset (IMMEDIATE_MASK, IMMEDIATE_SHIFT, VALUE); }


    void set_src_activation_flag (activation_flag_e VALUE) {                                   set_bitset (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_SHIFT, static_cast<unsigned int>(VALUE)); }
    void set_dst_activation_flag (activation_flag_e VALUE) {                                   set_bitset (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_SHIFT, static_cast<unsigned int>(VALUE)); }

  public:
    activation_flag_e get_src_activation_flag () const { return static_cast<activation_flag_e>(get_bitset (SRC_ACTIVATION_FLAG_MASK, SRC_ACTIVATION_FLAG_SHIFT)); }
    activation_flag_e get_dst_activation_flag () const { return static_cast<activation_flag_e>(get_bitset (DST_ACTIVATION_FLAG_MASK, DST_ACTIVATION_FLAG_SHIFT)); }

  };

  using CouplingWithData = Coupling; // FIXME to be removed when dust settle

}
