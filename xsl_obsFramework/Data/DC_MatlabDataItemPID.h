  
//
// Copyright 2003 P&P Software GmbH - All Rights Reserved
//
// DC_MatlabDataItemPID.h
//
// This file was generated automatically

#ifndef DC_MatlabDataItemPID_H
#define DC_MatlabDataItemPID_H

#include "../GeneralInclude/ForwardDeclarations.h"
#include "../GeneralInclude/BasicTypes.h"
#include "../Data/DataItemControlBlock.h"
#include "../Qom/object.h"

#ifdef HAVE_SIMULINK
extern "C" {
#include "MatlabAutocode/pid.h"
}
#endif

/**
 * Matlab Wrapper for a PID Control Block.
 * This class assumes that the Real-Time
 * Workshop code generation tool with which the wrapped code was generated
 * using the
 * "RTW Embedded Coder" target file. The storage class of all inputs,
 * outputs and parameters is assumed to have been set to "ExportedGlobal".
 * <p>
 * This class was tested only with code generated by Real-Time Workshop
 * Embedded Coder version 3.0.
 * <p>
 * The Matlab-generated code is not part of the OBS
 * Framework. It must be provided by the user. This class assumes that
 * the code is placed in directory: "../MatlabAutocode" where the path
 * is relative to the directory where this class is stored.
 * <p>
 * Since not all users will be able to provide this code, a compiler switch
 * is provided to disable use of the
 * matlab-generated code. If the macro HAVE_SIMULINK is
 * not defined, then the class behaves like a dummy class that
 * implements a trivial transfer function.
 * <p>
 * The wrapper is implemented as a form of <i>control block</i>.
 * The control block has 1 inputs and
 * 1 outputs.
 * The control block has the following parameters:
 * <ul>
 * <li>KP: The proportional gain</li>
 * <li>KI: The integral gain</li>
 * <li>KD: The derivative gain</li>
 * </ul>
 * The model "step" and "initialization" functions are:
 * <code>PID_step</code> and 
 * <code>PID_init</code>.
 * The model "terminate" function is never invoked (OBS classes are never
 * destroyed). The
 * data item link 
 * mechanism is used to link the Matlab inputs and outputs 
 * to their external inputs and outputs.
 * <p>
 * This class should be used as a singleton because the matlab-generated
 * code is not re-entrant. 
 * @see HAVE_SIMULINK
 * @author R. Totaro (code template) / A. Pasetti (code generator)
 * @version 1.0
 */

#define TYPE_DC_MatlabDataItemPID "DC_MatlabDataItemPID"

void DC_MatlabDataItemPID_register(void);


///////////////////////////////////////////////////////////////////////////////
//
//                            class and struct
//
///////////////////////////////////////////////////////////////////////////////

struct DC_MatlabDataItemPID {
    DataItemControlBlock parent;

    unsigned int numberOfOutputs;
    unsigned int numberOfInputs;
    unsigned int numberOfParameters;
    
    
#ifdef HAVE_SIMULINK
    /**
     * The following arrays contain pointers to the globally visible
     * input/output/parameter variables in the Simulink model.
     * The type real_T is defined in Simulink's header files.
     */
    real_T **modelInputs;
    real_T **modelOutputs;
    real_T **modelParameters;
/**
 * Pointers to the Simulink model update and initialization function
 * The type boolean_T is defined in Simulink's header files.
 */
    void (*modelStep)(void);
    void (*modelInitialize)(boolean_T);
#endif
};


struct DC_MatlabDataItemPIDClass {
    DataItemControlBlockClass parent_class;
 
    /**
     * Implement the state propagation function. State propagation is performed
     * internally to the matlab-generated code. This method therefore performs
     * only the following two housekeeping tasks:
     * Copy the values of the parameters from the buffer 
     * maintained by the control block class into the parameter buffer maintained by the
     * Matlab code
     * Load the values of the inputs into the input buffer maintained by the
     * Matlab code
     * 
     * @see ControlBlockFloatDbPar#propagate
     */
    void (*propagateState)(void *obj);
    /**
     * Implement the output update function. This method performs
     * the following two tasks:
     * Call the matlab-generated step function
     * Write the values of the outputs computed
     * by the matlab-generated code to the control block outputs
     * 
     * @see ControlBlock#propagate
     */
    void (*updateOutput)(void *obj);
    /**
     * Reset the control block by invoking the matlab-generated initialize function.
     */
    void (*reset)(void *obj);
};

DC_MatlabDataItemPID

#define DC_MatlabDataItemPID_GET_CLASS(obj) \
        OBJECT_GET_CLASS(DC_MatlabDataItemPIDClass, obj, TYPE_DC_MatlabDataItemPID)

#define DC_MatlabDataItemPID_CLASS(klass) \
        OBJECT_CLASS_CHECK(DC_MatlabDataItemPIDClass, klass, TYPE_DC_MatlabDataItemPID)

#define DC_MatlabDataItemPID(obj) \
        OBJECT_CHECK(DC_MatlabDataItemPID, obj, TYPE_DC_MatlabDataItemPID)


DC_MatlabDataItemPID* DC_MatlabDataItemPID_new(void);



#endif


#endif
