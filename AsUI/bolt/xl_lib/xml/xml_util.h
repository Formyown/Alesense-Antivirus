
/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, LTD. 2007                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   xml_util.h
*   Author      :   刘智聪(waterflier)
*   Create      :   2007年6月12日 
*   LastChange  :   
*   History     :	
*								
*   Description :  定义了一些常用的XML相关的函数
*
********************************************************************/  

#ifndef _XUNLEI_XML_UTIL_H_H
#define _XUNLEI_XML_UTIL_H_H

#ifdef __LINUX__

#else
#define snprintf _snprintf
#endif 

#include <stdio.h>
#include <stdlib.h>

namespace xl
{
namespace xml
{

class xml_util
{
public:
    
    static int xml_string_copy(const char* psrc,char* buffer,size_t bufferLen=0x7fffffff)
    {
        int nowWritePos = 0;
        
        while(*psrc)
        {
            char nowChar = *psrc;
            if(nowChar < 32 && nowChar > 0)
            {
                // &#xx; 
                if(bufferLen - nowWritePos < 5) return -1;
                buffer[nowWritePos] = '&';
                buffer[nowWritePos+1] = '#';
                buffer[nowWritePos+2] = '0' + nowChar / 10;
                buffer[nowWritePos+3] = '0' + nowChar % 10;
                buffer[nowWritePos + 4] =';';
                nowWritePos +=5;
            }
            else
            {
                switch(nowChar)
                {
                case '<':
                    //&lt;
                    if(bufferLen - nowWritePos < 4) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'l';
                    buffer[nowWritePos+2] = 't';
                    buffer[nowWritePos+3] = ';';
                    nowWritePos+=4;
                    break;
                case '>':
                    //&gt;
                    if(bufferLen - nowWritePos < 4) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'g';
                    buffer[nowWritePos+2] = 't';
                    buffer[nowWritePos+3] = ';';
                    nowWritePos+=4;
                    break;
                case '&':
                    //&amp; 
                    if(bufferLen - nowWritePos < 5) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'a';
                    buffer[nowWritePos+2] = 'm';
                    buffer[nowWritePos+3] = 'p';
                    buffer[nowWritePos+4] = ';';
                    nowWritePos+=5;
                    break;
                case '\'':
                    //&apos;
                    if(bufferLen - nowWritePos < 6) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'a';
                    buffer[nowWritePos+2] = 'p';
                    buffer[nowWritePos+3] = 'o';
                    buffer[nowWritePos+4] = 's';
                    buffer[nowWritePos+5] = ';';
                    nowWritePos+=6;
                    break;
                case '\"':
                    //&quot;
                    if(bufferLen - nowWritePos < 6) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'q';
                    buffer[nowWritePos+2] = 'u';
                    buffer[nowWritePos+3] = 'o';
                    buffer[nowWritePos+4] = 't';
                    buffer[nowWritePos+5] = ';';
                    nowWritePos+=6;
                    break;
                default:
                    if(bufferLen - nowWritePos < 1) return -1;
                    buffer[nowWritePos] = nowChar;
                    nowWritePos ++;
                }
            }
            psrc++;
        }
        buffer[nowWritePos] = 0;
        return nowWritePos;
    }
  
    static int xml_write_attr_value(const char* attrName,const char* attrValue,char* buffer,size_t bufferLen=0x7fffffff)
    {
        int len;
        len = snprintf(buffer,bufferLen," %s='",attrName);
        if(len == -1)
            return -1;

        buffer += len;
        bufferLen -= len;

        int nowWritePos = 0;
        
        while(*attrValue)
        {
            char nowChar = (unsigned char)*attrValue;
            if(nowChar < 32 && nowChar > 0)
            {
                // &#xx; 
                if(bufferLen - nowWritePos < 5) return -1;
                buffer[nowWritePos] = '&';
                buffer[nowWritePos+1] = '#';
                buffer[nowWritePos+2] = '0' + nowChar / 10;
                buffer[nowWritePos+3] = '0' + nowChar % 10;
                buffer[nowWritePos + 4] =';';
                nowWritePos +=5;
            }
            else
            {
                switch(nowChar)
                {
                case '<':
                    //&lt;
                    if(bufferLen - nowWritePos < 4) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'l';
                    buffer[nowWritePos+2] = 't';
                    buffer[nowWritePos+3] = ';';
                    nowWritePos+=4;
                    break;
                case '>':
                    //&gt;
                    if(bufferLen - nowWritePos < 4) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'g';
                    buffer[nowWritePos+2] = 't';
                    buffer[nowWritePos+3] = ';';
                    nowWritePos+=4;
                    break;
                case '&':
                    //&amp; 
                    if(bufferLen - nowWritePos < 5) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'a';
                    buffer[nowWritePos+2] = 'm';
                    buffer[nowWritePos+3] = 'p';
                    buffer[nowWritePos+4] = ';';
                    nowWritePos+=5;
                    break;
                case '\'':
                    //&apos;
                    if(bufferLen - nowWritePos < 6) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'a';
                    buffer[nowWritePos+2] = 'p';
                    buffer[nowWritePos+3] = 'o';
                    buffer[nowWritePos+4] = 's';
                    buffer[nowWritePos+5] = ';';
                    nowWritePos+=6;
                    break;
                case '\"':
                    //&quot;
                    if(bufferLen - nowWritePos < 6) return -1;
                    buffer[nowWritePos] = '&';
                    buffer[nowWritePos+1] = 'q';
                    buffer[nowWritePos+2] = 'u';
                    buffer[nowWritePos+3] = 'o';
                    buffer[nowWritePos+4] = 't';
                    buffer[nowWritePos+5] = ';';
                    nowWritePos+=6;
                    break;
                default:
                    if(bufferLen - nowWritePos < 1) return -1;
                    buffer[nowWritePos] = nowChar;
                    nowWritePos ++;
                }
            }
            attrValue++;
        }
        if(bufferLen - nowWritePos < 3)
            return -1;
        buffer[nowWritePos++] = '\'';
        buffer[nowWritePos++]=' ';
        buffer[nowWritePos] = 0;
            
        return nowWritePos + len;
    }
};

}//namespace xml
}//namespace xl
#endif //_XUNLEI_XML_UTIL_H_H
