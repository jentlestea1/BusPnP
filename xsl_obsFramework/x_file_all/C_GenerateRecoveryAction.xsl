<?xml version="1.0" encoding="UTF-8"?>
<!--
    Copyright 2004 P&P Software GmbH                                                                                 
-->
<!--
This is the code generator for the header and body files of a custom RecoveryAction class.
This XSL program process the XML-based application model.
The XSL program searches the application model for all "RecoveryAction" elements
which have a "Custom" subelement that indicates that a custom recovery action
class must be created. For each such element a class header file and a class
body files are created.
The base class of the generated class is assumed to be the class in the
"type" attribute of the RecoveryAction element.

NB: This program writes its outputs (the header and body files) to documents that are 
opened using the "xsl:document" instruction. The directory where these files
are written must apparently be specified through an absolute path names (this
seems to be in contrast with the documentation of xsl:document and may be a
bug in the XSLT provessor). This directory is hardcoded in variable
$TargetFile.

@todo: 
      - add treatment of default attributes
      - add setting of class identifier in the constructor body
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  version="1.1"  xmlns:lxslt="http://xml.apache.org/xslt">

<xsl:strip-space elements="*"/>
<xsl:output omit-xml-declaration="yes" method="text"/>

<!--
    Global Variables                                                                                                                      
-->
<xsl:variable name="codeDir" select="'../'"/>
<!--
       Include general utility rules
-->
<xsl:include href="WriteCommentBody.xsl"/>
<xsl:include href="ClassNameFromPath.xsl"/>
<!--
    Top-Level Rule (iterates over all RecoveryAction elements with a Custom subelement)
-->
<xsl:template match="/ObsApplication">
      <!-- write warning about the location of the output file -->
       WARNING: this XSL program hardcodes the directory where output files are written in variable $TargetFile
            
      <xsl:for-each select="//RecoveryAction[Custom]">
            <xsl:variable name="SuperClassName">
                  <xsl:call-template name="ClassNameFromPath">
                        <xsl:with-param name="path" select="@type"/>
                  </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="ClassName">
                  <xsl:call-template name="ClassNameFromPath">
                        <xsl:with-param name="path" select="Custom/@type"/>
                  </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="TargetFile" select="concat('/home/wangshaobo/obs_xml_xsl/projects_c/',Custom/@type)"/>     
            <xsl:variable name="LowerCase" select="'abcdefghijklmnopqrstuvwxyz'"/>
            <xsl:variable name="UpperCase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
            Generating code for class <xsl:value-of select="$ClassName"/> in files <xsl:value-of select="$TargetFile"/>.h/.cpp

            <!-- Check whether writing to an output file is possible -->
            <xsl:if test="not(element-available('xsl:document'))">
                  FATAL ERROR: the xsl:document element is not supported. Header files cannot be written! This element
                  is normally supported only in version 1.1 of XSL.
            </xsl:if>
            
            <!-- ===================================================================================
                   Create the header file  
                   ===================================================================================-->
             <xsl:document href="{$TargetFile}.h" omit-xml-declaration="yes" method="text">  
//
// Copyright 2004 P&amp;P Software GmbH - All Rights Reserved
//
// <xsl:value-of select="$ClassName"/>.h
//
// This file was automatically generated by an XSL program

#ifndef <xsl:value-of select="$ClassName"/>_H
#define <xsl:value-of select="$ClassName"/>_H

#include "<xsl:value-of select="$codeDir"/>GeneralInclude/ForwardDeclarations.h"
#include "<xsl:value-of select="$codeDir"/>GeneralInclude/BasicTypes.h"
#include "<xsl:value-of select="$codeDir"/><xsl:value-of select="@type"/>.h"
#include "../Qom/object.h"

/**
<xsl:call-template name="WriteCommentBody">
         <xsl:with-param name="text" select="normalize-space(Custom/Description)"/>
         <xsl:with-param name="indent" select="' '"/>
</xsl:call-template>
 * &lt;p&gt;
 * This is a stub class. It provides dummy implementations for some of the virtual
 * methods of its superclass. 
 * This class was automatically generated by an XSL program that processes the
 * XML-based &lt;i&gt;target application model&lt;/i&gt;. The XSL program searches 
 * the  searches the application model for all "RecoveryAction" elements
 * which have a "Custom" subelement that indicates that a custom recovery action
 * class must be created. For each such element a class header file is created.
 * The base class of the generated class is assumed to be the class in the
 * "type" attribute of the RecoveryAction element. 
 * The information in the application model is used to decide which virtual methods
 * should be overridden.
 * @todo Modify the generator meta-component generateRecoveryAction to generate the code
 * that sets the class identifier and to treat the default attributes in the custom recovery action description.
 * @author Automatically Generated Class
 * @version 1.0
 */
 <!--
class <xsl:value-of select="$ClassName"/> : public <xsl:value-of select="$SuperClassName"/> {

  public: 
  
    /**
     * Stub constructor that returns without taking any action.
     */
    <xsl:value-of select="$ClassName"/>(void);
    
    /**
     * This is a stub method that must be completed by the application developer.
     * This stub provides a default implementation that returns ACTION_SUCCESS but does not
     * otherwise take any action.
     * @return true if the continuation action of this state has terminated, false otherwise
     */
    virtual TD_ActionOutcome doRecoveryAction(void);
};
-->
#define TYPE_<xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/> "<xsl:value-of select="translate($ClassName,$UpperCase,$LowerCase)"/>"
void <xsl:value-of select="$ClassName"/>_register(void);


/////////////////////////////////////////////////////////////////////////////////
//
//                              class and struct
//
/////////////////////////////////////////////////////////////////////////////////

struct <xsl:value-of select="$ClassName"/> {
    <xsl:value-of select="$SuperClassName"/> parent;
};


struct <xsl:value-of select="$ClassName"/>Class {
    <xsl:value-of select="$SuperClassName"/>Class parent_class;
    /**
     * This is a stub method that must be completed by the application developer.
     * This stub provides a default implementation that returns ACTION_SUCCESS but does not
     * otherwise take any action.
     * @return true if the continuation action of this state has terminated, false otherwise
     */
    TD_ActionOutcome (*doRecoveryAction)(void *obj);

};


#define <xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>_GET_CLASS(obj) \
         OBJECT_GET_CLASS(<xsl:value-of select="$ClassName"/>Class, obj, TYPE_<xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>)


#define <xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>_CLASS(klass) \
         OBJECT_CLASS_CHECK(<xsl:value-of select="$ClassName"/>Class, klass, TYPE_<xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>)


#define <xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>(obj) \
         OBJECT_CHECK(<xsl:value-of select="$ClassName"/>, obj, TYPE_<xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>)


<xsl:value-of select="$ClassName"/>* <xsl:value-of select="$ClassName"/>_new(void);


#endif
             </xsl:document>
             
            <!-- ===================================================================================
                   Create the body file  
                   ===================================================================================-->
             <xsl:document href="{$TargetFile}.c" omit-xml-declaration="yes" method="text">  
//
// Copyright 2004 P&amp;P Software GmbH - All Rights Reserved
//
// <xsl:value-of select="$ClassName"/>.c
//
// This file was automatically generated by an XSL program

#include "<xsl:value-of select="$codeDir"/>GeneralInclude/CompilerSwitches.h"
#include "<xsl:value-of select="$codeDir"/>GeneralInclude/Constants.h"
#include "<xsl:value-of select="$codeDir"/><xsl:value-of select="@type"/>.h"
#include "<xsl:value-of select="$codeDir"/><xsl:value-of select="Custom/@type"/>.h"
<!--
<xsl:value-of select="$ClassName"/>::<xsl:value-of select="$ClassName"/>(void) {
      // insert application-specific code here
}
    
TD_ActionOutcome <xsl:value-of select="$ClassName"/>::doRecoveryAction(void) {
      // insert application-specific code here
      return ACTION_SUCCESS;
}
-->
/////////////////////////////////////////////////////////////////////////////////
//
//                    non-pure virtual  method(s) definition
//
/////////////////////////////////////////////////////////////////////////////////


static void doRecoveryAction(void *obj,TD_Float value)
{
      // insert application-specific code here
      return ACTION_SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////
//
//                   object constructor and destructor
//
///////////////////////////////////////////////////////////////////////////////

// the following may be useful if you don't need it, just delete.
// <xsl:value-of select="$ClassName"/> *this = <xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>(obj)
static void instance_init(Object *obj)
{
      // insert application-specific code here
}

<xsl:value-of select="$ClassName"/>* <xsl:value-of select="$ClassName"/>_new(void)
{
        return (<xsl:value-of select="$ClassName"/>*)object_new(TYPE_<xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>);

}



///////////////////////////////////////////////////////////////////////////////
//
//                   binding and type registration
//
///////////////////////////////////////////////////////////////////////////////

static void class_init(ObjectClass *oc, void *data)
{
<xsl:value-of select="$ClassName"/>Class *dc_p_class = <xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>_CLASS(oc);

        dc_p_class->doRecoveryAction = doRecoveryAction;

}

static const TypeInfo type_info = {
        .name = TYPE_<xsl:value-of select="translate($ClassName,$LowerCase,$UpperCase)"/>,
        .parent = TYPE_<xsl:value-of select="translate($SuperClassName,$LowerCase,$UpperCase)"/>,
        .instance_size = sizeof(<xsl:value-of select="$ClassName"/>),
        .abstract = false,
        .class_size = sizeof(<xsl:value-of select="$ClassName"/>Class),
        .instance_init = instance_init,
        .class_init = class_init,

};

void <xsl:value-of select="$ClassName"/>_register(void)
{
        type_register_static(&amp;type_info);

}


            </xsl:document>
      </xsl:for-each>
</xsl:template>




</xsl:stylesheet>
