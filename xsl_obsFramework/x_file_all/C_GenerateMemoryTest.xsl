<?xml version="1.0" encoding="UTF-8"?>
<!--
    Copyright 2004 P&P Software GmbH                                                                                 
-->
<!--
This is the code generator for the main program for the memory test on the ERC32.
This program processes the framework model file (the merged version that contains
all features). It identifies all component features and it creates a C++ main program 
that instantiates each component framework component once.
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
<xsl:include href="ClassNameFromPath.xsl"/>
<xsl:include href="TruncatedClassNameFromPath.xsl"/>
<!--
    Top-Level Rule 
-->
<xsl:template match="/FrameworkFeatureModel">
//
// Copyright 2004 P&amp;P Software GmbH - All Rights Reserved
//
// MemoryTest.cpp
//
// This file was automatically generated by an XSL program
            
#include "../GeneralInclude/CompilerSwitches.h"            
      <xsl:for-each select="//Component[(@status='provided')]">
#include "<xsl:value-of select="$codeDir"/><xsl:value-of select="@type"/>.h" <xsl:text/>           
      </xsl:for-each>
      
int main(int argc, char* argv[]) {

      <xsl:for-each select="//Component[(@status='provided')]">
            <xsl:variable name="ClassName">
                  <xsl:call-template name="ClassNameFromPath">
                        <xsl:with-param name="path" select="@type"/>
                  </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="TruncatedClassName">
                  <xsl:call-template name="TruncatedClassNameFromPath">
                        <xsl:with-param name="path" select="@type"/>
                  </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="$ClassName"/>* p<xsl:value-of select="$TruncatedClassName"/>;
       </xsl:for-each>
            
      <xsl:for-each select="//Component[(@status='provided')]">
            <xsl:variable name="ClassName">
                  <xsl:call-template name="ClassNameFromPath">
                        <xsl:with-param name="path" select="@type"/>
                  </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="TruncatedClassName">
                  <xsl:call-template name="TruncatedClassNameFromPath">
                        <xsl:with-param name="path" select="@type"/>
                  </xsl:call-template>
            </xsl:variable>
            <xsl:choose>
                  <xsl:when test="not(($ClassName='DC_DataItem') or ($ClassName='DC_SettableDataItem') or ($ClassName='DC_RawDataItem'))">
            p<xsl:value-of select="$TruncatedClassName"/> = new <xsl:value-of select="$ClassName"/>();  <xsl:text/>           
                  </xsl:when>
                  <xsl:otherwise>
            p<xsl:value-of select="$TruncatedClassName"/> = new <xsl:value-of select="$ClassName"/>((TD_Integer*)pNULL);  <xsl:text/>           
                  </xsl:otherwise>
            </xsl:choose>
            
      </xsl:for-each>
}      
</xsl:template>

</xsl:stylesheet>
