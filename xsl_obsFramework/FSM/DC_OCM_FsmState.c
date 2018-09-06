  
//
// Copyright 2004 P&P Software GmbH - All Rights Reserved
//
// DC_OCM_FsmState.c
//
// This file was automatically generated by an XSL program

#include "../GeneralInclude/CompilerSwitches.h"
#include "../GeneralInclude/Constants.h"
#include "../FSM/FsmState.h"
#include "../FSM/DC_OCM_FsmState.h"

/////////////////////////////////////////////////////////////////////////////////
//
//                    non-pure virtual  method(s) definition
//
/////////////////////////////////////////////////////////////////////////////////


    
static void doInit(void *obj) {
      return FSMSTATE_GET_CLASS(obj)->doInit((void*)(obj));
      // remove previous statement and insert application-specific code here
}
    
static void doContinue(void *obj) {
      return;
      // remove previous statement and insert application-specific code here
}
          
static bool isFinished(void *obj) {
      return DC_OCM_FSMSTATE_GET_CLASS(obj)->isFinished((void*)(obj));
      // remove previous statement and insert application-specific code here
}
    


///////////////////////////////////////////////////////////////////////////////
//
//                   object constructor and destructor
//
///////////////////////////////////////////////////////////////////////////////

// the following may be useful if you don't need it, just delete.
// DC_OCM_FsmState *this = DC_OCM_FSMSTATE(obj)
static void instance_init(Object *obj)
{
      // insert application-specific code here
}

DC_OCM_FsmState* DC_OCM_FsmState_new(void)
{
        return (DC_OCM_FsmState*)object_new(TYPE_DC_OCM_FSMSTATE);

}



///////////////////////////////////////////////////////////////////////////////
//
//                   binding and type registration
//
///////////////////////////////////////////////////////////////////////////////

static void class_init(ObjectClass *oc, void *data)
{
DC_OCM_FsmStateClass *dc_p_class = DC_OCM_FSMSTATE_CLASS(oc);

    
        dc_p_class->doInit=doInit;
    
        dc_p_class->doContinue=doContinue;
          
        dc_p_class->isFinished=isFinished;
    

}

static const TypeInfo type_info = {
        .name = TYPE_DC_OCM_FSMSTATE,
        .parent = TYPE_FSMSTATE,
        .instance_size = sizeof(DC_OCM_FsmState),
        .abstract = false,
        .class_size = sizeof(DC_OCM_FsmStateClass),
        .instance_init = instance_init,
        .class_init = class_init,

};

void DC_OCM_FsmState_register(void)
{
        type_register_static(&type_info);

}
            