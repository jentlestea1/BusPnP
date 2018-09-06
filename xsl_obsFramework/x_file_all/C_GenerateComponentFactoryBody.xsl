<?xml version="1.0" encoding="UTF-8"?>

<!--================================================================-->
<!-- Copyright 2004 P&P Software GmbH                                                                                  -->
<!--================================================================-->

<!--===================================================================
This is the code generator for the body file of the application component factory for the 
OBS Framework.

This XSL program process the XML-based application model.
===================================================================-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    version="1.1"  xmlns:lxslt="http://xml.apache.org/xslt"
    xmlns:exslt="http://exslt.org/common">

<xsl:strip-space elements="*"/>
<xsl:output omit-xml-declaration="yes" method="text"/>
<!--
    Global Variables                                                                                                                      
-->
<xsl:variable name="codeDir" select="'../'"/>
<xsl:variable name="className" select="'CC_ComponentFactory'"/>
<xsl:variable name="numberOfComponents" select="count(//*[(@kind='component') and (@name)])"/>
<xsl:variable name="LowerCase" select="'abcdefghijklmnopqrstuvwxyz'"/>
<xsl:variable name="UpperCase" seletc="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
<!--
       Include general utility rules
-->

<xsl:include href="WriteCommentBody.xsl"/>
<xsl:include href="ClassNameFromPath.xsl"/>
<xsl:include href="TruncatedClassNameFromPath.xsl"/>
<xsl:include href="TypeNameFromPath.xsl"/>
<xsl:include href="ConfigureObsClock.xsl"/>
<xsl:include href="ConfigureEventRepository.xsl"/>
<xsl:include href="ConfigureFSM.xsl"/>
<xsl:include href="ConfigureControlBlock.xsl"/>
<xsl:include href="ConfigureControlAction.xsl"/>
<xsl:include href="ConfigureDataPool.xsl"/>
<xsl:include href="ConfigureParameterDatabase.xsl"/>
<xsl:include href="ConfigureRootObject.xsl"/>
<xsl:include href="ConfigureFunctionalityManagers.xsl"/>
<!--
-->

<!--
    Top-Level Rule                                                 
-->
<xsl:template match="/ObsApplication">
<xsl:variable name="TargetFile" select="'/home/wangshaobo/obs_xml_xsl/projects_c/componentFactory/CC_ComponentFactory'"/>
<xsl:document href="{$TargetFile}.c" omit-xml-declaration="yes" method="text">
//
// Copyright 2004 P&amp;P Software GmbH - All Rights Reserved
//
// <xsl:value-of select="$className"/>.c
//
// This file was automatically generated by an XSL program

#include "<xsl:value-of select="$codeDir"/>GeneralInclude/ForwardDeclarations.h"
#include "<xsl:value-of select="$codeDir"/>GeneralInclude/CompilerSwitches.h"
#include "<xsl:value-of select="$codeDir"/>GeneralInclude/DebugSupport.h"
#include "<xsl:value-of select="$codeDir"/>GeneralInclude/BasicTypes.h"
#include "<xsl:value-of select="$codeDir"/>GeneralInclude/Constants.h"
#include "<xsl:value-of select="$className"/>_inc.h"
#include "<xsl:value-of select="$className"/>.h"
#include "<xsl:value-of select="$codeDir"/>Base/CC_RootObject.h"   

<xsl:variable name="temp">
<xsl:for-each select="//*[(@kind='component') and (@name)]">  
   <xsl:element name="type"><xsl:value-of select="@type"/></xsl:element>
</xsl:for-each>
</xsl:variable>

<xsl:variable name="sortedTemp">
<xsl:for-each select="exslt:node-set($temp)/type">
    <xsl:sort/>
   <xsl:element name="type"><xsl:value-of select="."/></xsl:element>
</xsl:for-each>
</xsl:variable>

<xsl:for-each select="exslt:node-set($sortedTemp)/type">
      <xsl:variable name="value" select="."/>
      <xsl:variable name="count" select="position()"/>
      <xsl:variable name="previousValue" select="../type[position()=number($count)-1]"/>
      <xsl:if test="not($value=$previousValue)">
#include "<xsl:value-of select="$codeDir"/><xsl:value-of select="$value"/>.h"        <xsl:text/>
      </xsl:if>
</xsl:for-each>

// Disable type mismatch warning that arise when calling
// the configuration methods on monitoring profiles
#pragma warning( disable : 4305 )

<xsl:variable name="ParDatabaseSize">
<xsl:choose>
      <xsl:when test="//BasicDatabase">
            <xsl:value-of select="//DatabaseSize/@value"/>
      </xsl:when>
      <xsl:when test="//IndexedDatabase">
            <xsl:value-of select="//IndexedDatabase/Size/@value"/>
      </xsl:when>
      <xsl:otherwise>
             ERROR: no size information for the parameter database
      </xsl:otherwise>
</xsl:choose>
</xsl:variable>

// Create the parameter database storage areas. In an operational
// context the two pointers should be made to point to the database
// operational and default tables. The names of the pointers must 
// match the names in the parameter database application models.
char* PARDATABASE_OP_TABLE_START = (char*)malloc(<xsl:value-of select="$ParDatabaseSize"/>*sizeof(char));
char* PARDATABASE_DEF_TABLE_START = (char*)malloc(<xsl:value-of select="$ParDatabaseSize"/>*sizeof(char));

// Create the telecommand storage area. In an operational setting,
// this should be linked to the area where TCs are written. The name
// used here must match the name used in the telecommand loader model and
// the size of the allocated memory should be consistent with the configuration
// parameters of the telecommand loader.
unsigned char* TC_LOADER_AREA_START = new unsigned char[1000];

// Create the telemetry storage area. In an operational setting,
// this should be linked to the area from where TMs are read. The name
// used here must match the name used in the telemetry stream model and
// the size of the allocated memory should be consistent with the configuration
// parameters of the telemetry stream.

unsigned char* TM_STREAM_START = (unsigned char*)malloc(5000*sizeof(unsigned char));

///////////////////////////////////////////////////////////////////////////////
//
//                            class data
//
///////////////////////////////////////////////////////////////////////////////


<xsl:value-of select="$className"/>* pInstance = pNULL;


///////////////////////////////////////////////////////////////////////////////
//
//                            class  method(s) definition
//
///////////////////////////////////////////////////////////////////////////////


<xsl:value-of select="$className"/>* <xsl:value-of select="$className"/>_getInstance(void) {
   if (pInstance==pNULL)
      pInstance = <xsl:value-of select="$className"/>_new();
   return pInstance;
}


///////////////////////////////////////////////////////////////////////////////
//
//                  non-virtual member method(s) definition
//
///////////////////////////////////////////////////////////////////////////////



void <xsl:value-of select="$className"/>_configureApplication(<xsl:value-of select="$className"/> *this) {

   <xsl:value-of select="$className"/>_configureRootObject(this);
   <xsl:if test="EventRepository"><xsl:value-of select="$className"/>_configureEventRepository(this);</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="ObsClock"><xsl:value-of select="$className"/>_configureObsClock(this);</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="DataPool"><xsl:value-of select="$className"/>_configureDataPool(this);</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="ParameterDatabase"><xsl:value-of select="$className"/>_configureParameterDatabase(this);</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="FSM"><xsl:value-of select="$className"/>_configureFSM(this);</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="FSM"><xsl:value-of select="$className"/>_configureFunctionalityManagers(this);</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="ControllerManagement"><xsl:value-of select="$className"/>_configureControllers(this);</xsl:if>
   }

void <xsl:value-of select="$className"/>_configureRootObject(<xsl:value-of select="$className"/> *this) {
   <xsl:call-template name="configureRootObject"/>
}

<xsl:if test="ObsClock">
void <xsl:value-of select="$className"/>_configureObsClock(<xsl:value-of select="$className"/> *this) {
   <xsl:apply-templates select="ObsClock"/>
}
</xsl:if>

<xsl:if test="EventRepository">
void <xsl:value-of select="$className"/>_configureEventRepository(<xsl:value-of select="$className"/> *this) {
   <xsl:apply-templates select="EventRepository"/>
}
</xsl:if>

<xsl:if test="FSM">
void <xsl:value-of select="$className"/>_configureFSM(<xsl:value-of select="$className"/> *this) {
   <xsl:apply-templates select="FSM"/>
<xsl:text>
   </xsl:text>// 
<xsl:text/>
   <xsl:for-each select="//FSM">
            <xsl:variable name="fsm" select="@name"/>
             <xsl:value-of select="translate('CC_FSM',$LowerCase,$UpperCase)"/>_GET_CLASS(this-><xsl:value-of select="$fsm"/>)->reset();
<xsl:text/>
   </xsl:for-each>
}
</xsl:if>

<xsl:if test="ControllerManagement">
void <xsl:value-of select="$className"/>_configureControllers(<xsl:value-of select="$className"/> *this) {
      <xsl:apply-templates select="ControllerManagement/ControlAction/ControlBlock"/>           
      <xsl:apply-templates select="ControllerManagement/ControlAction"/>      
}     
</xsl:if>


 <xsl:if test="DataPool">
void <xsl:value-of select="$className"/>_configureDataPool(<xsl:value-of select="$className"/> *this) {
   <xsl:apply-templates select="DataPool"/>
}
</xsl:if>

 <xsl:if test="ParameterDatabase">
void <xsl:value-of select="$className"/>_configureParameterDatabase(<xsl:value-of select="$className"/> *this) {
   <xsl:apply-templates select="ParameterDatabase"/>
}
</xsl:if>

void <xsl:value-of select="$className"/>_configureFunctionalityManagers(<xsl:value-of select="$className"/> *this) {
   <xsl:apply-templates select="ManoeuvreManagement/ManoeuvreManager"/>
   <xsl:apply-templates select="TelecommandManagement/TelecommandManager"/>
   <xsl:apply-templates select="TelemetryManagement/TelemetryManager"/>
}

///////////////////////////////////////////////////////////////////////////////
//
//                   object constructor and destructor
//
///////////////////////////////////////////////////////////////////////////////


// the following may be useful if you don't need it, just delete.
// <xsl:value-of select="$className"/> *this = <xsl:value-of select="translate($className,$LowerCase,$UpperCase)"/>(obj)
static void instance_init(Object *obj)
{
   <xsl:variable name="numberOfObjects" select="count(//*[@kind='component'][@name])"/>
   // Initialize the system list size. The size is computed as the number of components
   // defined in the application model plus a (heuritically defined) margin of ten to
   // account for components that are created internally to other components
   CC_RootObject_setSystemListSize(<xsl:value-of select="$numberOfObjects"/>+10);

<xsl:for-each select="//*[(@kind='component') and (@name)]">  
      <xsl:variable name="className">
            <xsl:call-template name="ClassNameFromPath">
                  <xsl:with-param name="path" select="@type"/>
            </xsl:call-template>
      </xsl:variable>    
      <xsl:variable name="TypeName">
            <xsl:call-template name="TypeNameFromPath">
                  <xsl:with-param name="path" select="@type"/>
            </xsl:call-template>
      </xsl:variable>    
      <xsl:text>   </xsl:text><xsl:value-of select="@name"/> = <xsl:value-of select="$className"/>_new();
    <xsl:value-of select="$className"/>(obj)->list[<xsl:value-of  select="position()"/>] = <xsl:value-of select="@name"/>;
    <xsl:value-of select="$className"/>(obj)->type[<xsl:value-of  select="position()"/>] =  <xsl:value-of select="$TypeName"/>;
</xsl:for-each>

}

<xsl:value-of select="$className"/>* <xsl:value-of select="$className"/>_new(void)
{
        return (<xsl:value-of select="$className"/>*)object_new(TYPE_<xsl:value-of select="translate($className,$LowerCase,$UpperCase)"/>);

}



///////////////////////////////////////////////////////////////////////////////
//
//                   binding and type registration
//
///////////////////////////////////////////////////////////////////////////////

static void class_init(ObjectClass *oc, void *data)
{
<xsl:value-of select="$className"/>Class *dc_p_class = <xsl:value-of select="translate($className,$LowerCase,$UpperCase)"/>_CLASS(oc);

}

static const TypeInfo type_info = {
        .name = TYPE_<xsl:value-of select="translate($className,$LowerCase,$UpperCase)"/>,
        .parent = Object,
        .instance_size = sizeof(<xsl:value-of select="$className"/>),
        .abstract = false,
        .class_size = sizeof(<xsl:value-of select="$className"/>Class),
        .instance_init = instance_init,
        .class_init = class_init,

};

void <xsl:value-of select="$className"/>_register(void)
{
        type_register_static(&amp;type_info);

}



<!--
<xsl:value-of select="$className"/>::<xsl:value-of select="$className"/>(void) {

   <xsl:variable name="numberOfObjects" select="count(//*[@kind='component'][@name])"/>
   // Initialize the system list size. The size is computed as the number of components
   // defined in the application model plus a (heuritically defined) margin of ten to
   // account for components that are created internally to other components
   CC_RootObject::setSystemListSize(<xsl:value-of select="$numberOfObjects"/>+10);

<xsl:for-each select="//*[(@kind='component') and (@name)]">  
      <xsl:variable name="$className">
            <xsl:call-template name="$classNameFromPath">
                  <xsl:with-param name="path" select="@type"/>
            </xsl:call-template>
      </xsl:variable>    
      <xsl:variable name="TypeName">
            <xsl:call-template name="TypeNameFromPath">
                  <xsl:with-param name="path" select="@type"/>
            </xsl:call-template>
      </xsl:variable>    
      <xsl:text>   </xsl:text><xsl:value-of select="@name"/> = new <xsl:value-of select="$ClassName"/>();
   list[<xsl:value-of  select="position()"/>] = <xsl:value-of select="@name"/>;
   type[<xsl:value-of  select="position()"/>] =  <xsl:value-of select="$TypeName"/>;
</xsl:for-each>
}


<xsl:value-of select="$className"/>* <xsl:value-of select="$className"/>::getInstance(void) {
   if (pInstance==pNULL)
      pInstance = new <xsl:value-of select="$className"/>();
   return pInstance;
}

void <xsl:value-of select="$className"/>::configureApplication(void) {
   configureRootObject();
   <xsl:if test="EventRepository">configureEventRepository();</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="ObsClock">configureObsClock();</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="DataPool">configureDataPool();</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="ParameterDatabase">configureParameterDatabase();</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="FSM">configureFSM();</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="FSM">configureFunctionalityManagers();</xsl:if><xsl:text>
   </xsl:text>   <xsl:if test="ControllerManagement">configureControllers();</xsl:if>
}

void <xsl:value-of select="$className"/>::configureRootObject(void) {
   <xsl:call-template name="configureRootObject"/>
}

<xsl:if test="ObsClock">
void <xsl:value-of select="$className"/>::configureObsClock(void) {
   <xsl:apply-templates select="ObsClock"/>
}
</xsl:if>

<xsl:if test="EventRepository">
void <xsl:value-of select="$className"/>::configureEventRepository(void) {
   <xsl:apply-templates select="EventRepository"/>
}
</xsl:if>

<xsl:if test="FSM">
void <xsl:value-of select="$className"/>::configureFSM(void) {
   <xsl:apply-templates select="FSM"/>
<xsl:text>
   </xsl:text>// 
<xsl:text/>
   <xsl:for-each select="//FSM">
            <xsl:variable name="fsm" select="@name"/>
            <xsl:text>   </xsl:text><xsl:value-of select="$fsm"/>->reset();
<xsl:text/>
   </xsl:for-each>
}
</xsl:if>

<xsl:if test="ControllerManagement">
void <xsl:value-of select="$className"/>::configureControllers(void) {
      <xsl:apply-templates select="ControllerManagement/ControlAction/ControlBlock"/>           
      <xsl:apply-templates select="ControllerManagement/ControlAction"/>           
}     
</xsl:if>

 <xsl:if test="DataPool">
void <xsl:value-of select="$className"/>::configureDataPool(void) {
   <xsl:apply-templates select="DataPool"/>
}
</xsl:if>

 <xsl:if test="ParameterDatabase">
void <xsl:value-of select="$className"/>::configureParameterDatabase(void) {
   <xsl:apply-templates select="ParameterDatabase"/>
}
</xsl:if>

void <xsl:value-of select="$className"/>::configureFunctionalityManagers(void) {
   <xsl:apply-templates select="ManoeuvreManagement/ManoeuvreManager"/>
   <xsl:apply-templates select="TelecommandManagement/TelecommandManager"/>
   <xsl:apply-templates select="TelemetryManagement/TelemetryManager"/>
}

-->
</xsl:document>
</xsl:template>

<xsl:template match="*|@*|text()">
</xsl:template>



</xsl:stylesheet>