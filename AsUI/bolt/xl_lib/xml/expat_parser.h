
/********************************************************************
*
* =-----------------------------------------------------------------=
* =                                                                 =
* =             Copyright (c) Xunlei, LTD. 2009                     =
* =                                                                 =
* =-----------------------------------------------------------------=
* 
*   FileName    :   expat_parser.h
*   Author      :   刘智聪(liuzhicong@xunlei.com)
*   Create      :   8:1:2009  
*   LastChange  :   2009/01/08
*   History     :	
*								
*   Description :   
*
********************************************************************/
/********************************************************************
*
* =-----------------------------------------------------------------=
* =                          ____          _________                =
* =                         / _  \         \___  __/                =
* =                        / /_/ /     ____   / /                   =
* =                       /  _   \ ● / _   \ / /                    =
* =                      /  /_/  // // / / // /                     =
* =                      \______//_//_/ /_//_/                      =
* =                                                                 =
* =             Copyright (c) BIN Technology studio,2004 - 2006     =
* =                           LET'Z BT                              =
* =-----------------------------------------------------------------=
* 
*   FileName    :   exmpatParser.hpp 
*   Author      :   waterflier (waterflier@gmail.com)
*   Create      :   2006-11-10 16:08
*   LastChange  :   2006-11-10 16:08
*   History     :	
*								
*   Description :   class "expatParser"
*                   expatParser是BINT framework的XML SAX解析的基础封装类
*                   封装了一个快速的SAX XML解析器expat
*                   要使用 XML相关的功能,必须保证expat的头文件在你的路径设置中
*                   并且在使用到的项目中link到expat的library(libexpat.lib 静态 static,XMLParser.lib 动态)
* 
* Tip: 关于Expat的编码问题
* Although expat may accept
* input in various encodings, the strings that it passes
* to the handlers are always encoded in UTF-8. The
* application is responsible for any translation of
* these strings into other encodings. Also no matter
* what you put in, as long as it is in a known encoding,
* what you get out is UTF-8.
*
********************************************************************/  
 

//-----------------------------------------------------------------------------
// 
// @doc
//
// @module	ExpatImpl.h - Expat class container |
//
// This module contains the definition of the expat class container.
//
// Copyright (c) 1994-2002 - Descartes Systems Sciences, Inc.
//
// @end
//
// $History: ExpatImpl.h $
//      
//      *****************  Version 1  *****************
//      User: Tim Smith    Date: 1/29/02    Time: 1:57p
//      Created in $/Omni_V2/_ToolLib
//      1. String.h now replaced with StringCode.h.
//      2. StringRsrc.h modified to use new string class.
//      3. Added tons of new classes from the wedge work.
//      
//-----------------------------------------------------------------------------

#ifndef _EXPAT_PARSER_HPP_
#define _EXPAT_PARSER_HPP_


extern "C"
{
#include <expat/expat.h>
}

//-----------------------------------------------------------------------------
//
// Forward definitions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Template class definition
//
//-----------------------------------------------------------------------------
namespace xl
{

namespace xml
{

#ifndef __cdecl
#define __cdecl
#endif

template <class _T>
class expat_parser
{

// @access Constructors and destructors
public:
	
	// @cmember General constructor

	expat_parser ()
	{
		m_p = NULL;
	}

	// @cmember Destructor

	~expat_parser ()
	{
		destroy ();
	}

// @access Parser creation and deletion methods
public:

	// @cmember Create a parser

	bool create (const XML_Char *pszEncoding = NULL, 
		const XML_Char *pszSep = NULL)
	{

		//
		// Destroy the old parser
		//

		destroy ();

		//
		// If the encoding or seperator are empty, then NULL
		//

		if (pszEncoding != NULL && pszEncoding [0] == 0)
			pszEncoding = NULL;
		if (pszSep != NULL && pszSep [0] == 0)
			pszSep = NULL;

		//
		// Create the new one
		//

		m_p = XML_ParserCreate_MM (pszEncoding, NULL, pszSep);
		if (m_p == NULL)
			return false;

		//
		// Invoke the post create routine
		//

		_T *pThis = static_cast <_T *> (this);
		pThis ->OnPostCreate ();

		//
		// Set the user data used in callbacks
		//

		XML_SetUserData (m_p, (void *) this);
		return true;
	}

	// @cmember Destroy the parser

	void destroy ()
	{
		if (m_p != NULL)
			XML_ParserFree (m_p);
		m_p = NULL;
	}

// @access Parser parse methods
public:
    
	// @cmember Parse a block of data
    
	bool parse (const char *pszBuffer, int nLength = -1, bool fIsFinal = true)
	{

		//
		// Validate
		//

		//assert (m_p != NULL);

		//
		// Get the length if not specified
		//

		if (nLength < 0)
			nLength = (int)strlen (pszBuffer);

		//
		// Invoke the parser
		//

		return XML_Parse (m_p, pszBuffer, nLength, fIsFinal) != 0;
	}

	// @cmember Parse a block of data

#ifdef WCHAR
	bool parse (const WCHAR *pszBuffer, int nLength = -1, bool fIsFinal = true)
	{

		//
		// Validate
		//

		//assert (m_p != NULL);

		//
		// Get the length if not specified
		//

		if (nLength < 0)
			nLength = wcslen (pszBuffer) * 2;

		//
		// Invoke the parser
		//

		return XML_Parse (m_p, pszBuffer, nLength, fIsFinal) != 0;
	}
#endif

	// @cmember Parse internal buffer

	bool parse_buffer (int nLength, bool fIsFinal = true)
	{
		//assert (m_p != NULL);
		return XML_ParseBuffer (m_p, nLength, fIsFinal) != 0;
	}

	// @cmember Get the internal buffer

	void *get_buffer (int nLength)
	{
		//assert (m_p != NULL);
		return XML_GetBuffer (m_p, nLength);
	}

    int stop_parser(bool resumable = false)
    {
        return XML_StopParser(m_p,resumable);
    }


// @access Parser callback enable/disable methods
public:

	// @cmember Enable/Disable the start element handler

	void enable_start_element_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetStartElementHandler (m_p, fEnable ? StartElementHandler : NULL);
	}

	// @cmember Enable/Disable the end element handler

	void enable_end_element_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetEndElementHandler (m_p, fEnable ? EndElementHandler : NULL);
	}

	// @cmember Enable/Disable the element handlers

	void enable_element_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		enable_start_element_handler (fEnable);
		enable_end_element_handler (fEnable);
	}

	// @cmember Enable/Disable the character data handler

	void enable_character_data_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetCharacterDataHandler (m_p,
			fEnable ? CharacterDataHandler : NULL);
	}

	// @cmember Enable/Disable the processing instruction handler

	void enable_processing_instruction_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetProcessingInstructionHandler (m_p, 
			fEnable ? ProcessingInstructionHandler : NULL);
	}

	// @cmember Enable/Disable the comment handler

	void enable_comment_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetCommentHandler (m_p, fEnable ? CommentHandler : NULL);
	}

	// @cmember Enable/Disable the start CDATA section handler

	void enable_start_cdata_section_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetStartCdataSectionHandler (m_p, 
			fEnable ? StartCdataSectionHandler : NULL);
	}

	// @cmember Enable/Disable the end CDATA section handler

	void enable_end_cdata_section_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetEndCdataSectionHandler (m_p, 
			fEnable ? EndCdataSectionHandler : NULL);
	}

	// @cmember Enable/Disable the CDATA section handlers

	void enable_cdata_section_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		enable_start_cdata_section_handler (fEnable);
		enable_end_cdata_section_handler (fEnable);
	}

	// @cmember Enable/Disable default handler
	
	void enable_default_handler (bool fEnable = true, bool fExpand = true)
	{
		//assert (m_p != NULL);
		if (fExpand)
		{
			XML_SetDefaultHandlerExpand (m_p, 
				fEnable ? DefaultHandler : NULL);
		}
		else
			XML_SetDefaultHandler (m_p, fEnable ? DefaultHandler : NULL);
	}
	
	// @cmember Enable/Disable external entity ref handler
	
	void enable_external_entity_ref_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetExternalEntityRefHandler (m_p, (XML_ExternalEntityRefHandler)
			fEnable ? ExternalEntityRefHandler : NULL);
	}
	
	// @cmember Enable/Disable unknown encoding handler
	void enable_unknown_encoding_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetUnknownEncodingHandler (m_p, 
			fEnable ? UnknownEncodingHandler : NULL,NULL);
	}
	
	// @cmember Enable/Disable start namespace handler
	
	void enable_start_namespace_decl_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetStartNamespaceDeclHandler (m_p, 
			fEnable ? StartNamespaceDeclHandler : NULL);
	}
	
	// @cmember Enable/Disable end namespace handler
	
	void enable_end_namespace_decl_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetEndNamespaceDeclHandler (m_p, 
			fEnable ? EndNamespaceDeclHandler : NULL);
	}

	// @cmember Enable/Disable namespace handlers

	void enable_namespace_decl_handler (bool fEnable = true)
	{
		enable_start_namespace_decl_handler (fEnable);
		enable_end_namespace_decl_handler (fEnable);
	}
	
	// @cmember Enable/Disable the XML declaration handler

	void enable_xml_decl_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetXmlDeclHandler (m_p, fEnable ? XmlDeclHandler : NULL);
	}

	// @cmember Enable/Disable the start DOCTYPE declaration handler

	void enable_start_doctype_decl_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetStartDoctypeDeclHandler (m_p, 
			fEnable ? StartDoctypeDeclHandler : NULL);
	}

	// @cmember Enable/Disable the end DOCTYPE declaration handler

	void enable_end_doctype_decl_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		XML_SetEndDoctypeDeclHandler (m_p, 
			fEnable ? EndDoctypeDeclHandler : NULL);
	}

	// @cmember Enable/Disable the DOCTYPE declaration handler

	void enable_doctype_decl_handler (bool fEnable = true)
	{
		//assert (m_p != NULL);
		enable_start_doctype_decl_handler (fEnable);
		enable_end_doctype_decl_handler (fEnable);
	}

// @access Parser error reporting methods
public:

	// @cmember Get last error

	enum XML_Error get_error_code () 
	{
		//assert (m_p != NULL);
		return XML_GetErrorCode (m_p);
	}

	// @cmember Get the current byte index

	long get_current_byte_index () 
	{
		//assert (m_p != NULL);
		return XML_GetCurrentByteIndex (m_p);
	}

	// @cmember Get the current line number

	int get_current_line_number () 
	{
		//assert (m_p != NULL);
		return XML_GetCurrentLineNumber (m_p);
	}

	// @cmember Get the current column number

	int get_current_column_number () 
	{
		//assert (m_p != NULL);
		return XML_GetCurrentColumnNumber (m_p);
	}

	// @cmember Get the current byte count

	int get_current_byte_count () 
	{
		//assert (m_p != NULL);
		return XML_GetCurrentByteCount (m_p);
	}

	// @cmember Get the input context

	const char *get_input_context (int *pnOffset, int *pnSize)
	{
		//assert (m_p != NULL);
		return XML_GetInputContext (m_p, pnOffset, pnSize);
	}

	// @cmember Get last error string

	const XML_LChar *get_error_string () 
	{
		return XML_ErrorString (get_error_code ());
	}

// @access Parser other methods
public:

	// @cmember Return the version string

	static const XML_LChar *get_expat_version ()
	{
		return XML_ExpatVersion ();
	}
	// @cmember Get last error string

	static const XML_LChar *get_error_string (enum XML_Error nError) 
	{
		return XML_ErrorString (nError);
	}

// @access Public handler methods
public:

	// @cmember Start element handler

	void OnStartElement (const XML_Char *pszName, const XML_Char **papszAttrs)
	{
		return;
	}

	// @cmember End element handler

	void OnEndElement (const XML_Char *pszName)
	{
		return;
	}

	// @cmember Character data handler

	void OnCharacterData (const XML_Char *pszData, int nLength)
	{
		return;
	}

	// @cmember Processing instruction handler

	void OnProcessingInstruction (const XML_Char *pszTarget, 
		const XML_Char *pszData)
	{
		return;
	}

	// @cmember Comment handler

	void OnComment (const XML_Char *pszData)
	{
		return;
	}

	// @cmember Start CDATA section handler

	void OnStartCdataSection ()
	{
		return;
	}

	// @cmember End CDATA section handler

	void OnEndCdataSection ()
	{
		return;
	}

	// @cmember Default handler
	
	void OnDefault (const XML_Char *pszData, int nLength)
	{
		return;
	}
	
	// @cmember External entity ref handler
	
	bool OnExternalEntityRef (const XML_Char *pszContext,
		const XML_Char *pszBase, const XML_Char *pszSystemID,
		const XML_Char *pszPublicID)
	{
		return false;
	}
	
	// @cmember Unknown encoding handler
	
	bool OnUnknownEncoding (const XML_Char *pszName, XML_Encoding *pInfo)
	{
		return false;
	}
	
	// @cmember Start namespace declaration handler
	
	void OnStartNamespaceDecl (const XML_Char *pszPrefix, 
		const XML_Char *pszURI)
	{
		return;
	}
	
	// @cmember End namespace declaration handler
	
	void OnEndNamespaceDecl (const XML_Char *pszPrefix)
	{
		return;
	}
	
	// @cmember XML declaration handler

	void OnXmlDecl (const XML_Char *pszVersion, const XML_Char *pszEncoding,
		bool fStandalone)
	{
		return;
	}

	// @cmember Start DOCTYPE declaration handler

	void OnStartDoctypeDecl (const XML_Char *pszDoctypeName, 
		const XML_Char *pszSysID, const XML_Char *pszPubID,
		bool fHasInternalSubset)
	{
		return;
	}

	// @cmember End DOCTYPE declaration handler

	void OnEndDoctypeDecl ()
	{
		return;
	}

// @access Protected methods
protected:

	// @cmember Handle any post creation

	void OnPostCreate ()
	{
	}

// @access Protected static methods
protected:

	// @cmember Start element handler wrapper

	static void __cdecl StartElementHandler (void *pUserData,
		const XML_Char *pszName, const XML_Char **papszAttrs)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnStartElement (pszName, papszAttrs);
	}

	// @cmember End element handler wrapper

	static void __cdecl EndElementHandler (void *pUserData,
		const XML_Char *pszName)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnEndElement (pszName);
	}

	// @cmember Character data handler wrapper

	static void __cdecl CharacterDataHandler (void *pUserData,
		const XML_Char *pszData, int nLength)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnCharacterData (pszData, nLength);
	}

	// @cmember Processing instruction handler wrapper

	static void __cdecl ProcessingInstructionHandler (void *pUserData,
		const XML_Char *pszTarget, const XML_Char *pszData)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnProcessingInstruction (pszTarget, pszData);
	}

	// @cmember Comment handler wrapper

	static void __cdecl CommentHandler (void *pUserData,
		const XML_Char *pszData)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnComment (pszData);
	}

	// @cmember Start CDATA section wrapper

	static void __cdecl StartCdataSectionHandler (void *pUserData)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnStartCdataSection ();
	}

	// @cmember End CDATA section wrapper

	static void __cdecl EndCdataSectionHandler (void *pUserData)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnEndCdataSection ();
	}

	// @cmember Default wrapper
	
	static void __cdecl DefaultHandler (void *pUserData, 
		const XML_Char *pszData, int nLength)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnDefault (pszData, nLength);
	}
	
	// @cmember External entity ref wrapper
	
	static int __cdecl ExternalEntityRefHandler ( XML_Parser parser, 
		const XML_Char *pszContext, const XML_Char *pszBase, 
		const XML_Char *pszSystemID, const XML_Char *pszPublicID)
	{
        void* pUserData = XML_GetUserData(parser);
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		return pThis ->OnExternalEntityRef (pszContext, 
			pszBase, pszSystemID, pszPublicID) ? 1 : 0;
	}
	
	// @cmember Unknown encoding wrapper
	
	static int __cdecl UnknownEncodingHandler (void *pUserData, 
		const XML_Char *pszName, XML_Encoding *pInfo)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		return pThis ->OnUnknownEncoding (pszName, pInfo) ? 1 : 0;
	}
	
	// @cmember Start namespace decl wrapper
	
	static void __cdecl StartNamespaceDeclHandler (void *pUserData, 
		const XML_Char *pszPrefix, const XML_Char *pszURI)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnStartNamespaceDecl (pszPrefix, pszURI);
	}
	
	// @cmember End namespace decl wrapper
	
	static void __cdecl EndNamespaceDeclHandler (void *pUserData, 
		const XML_Char *pszPrefix)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnEndNamespaceDecl (pszPrefix);
	}
	
	// @cmember XML declaration wrapper

	static void __cdecl XmlDeclHandler (void *pUserData,
		const XML_Char *pszVersion, const XML_Char *pszEncoding,
		int nStandalone)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnXmlDecl (pszVersion, pszEncoding, nStandalone != 0);
	}

	// @cmember Start Doctype declaration wrapper

	static void __cdecl StartDoctypeDeclHandler (void *pUserData,
		const XML_Char *pszDoctypeName, const XML_Char *pszSysID, 
		const XML_Char *pszPubID, int nHasInternalSubset)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnStartDoctypeDecl (pszDoctypeName, pszSysID, 
			pszPubID, nHasInternalSubset != 0);
	}

	// @cmember End Doctype declaration wrapper

	static void __cdecl EndDoctypeDeclHandler (void *pUserData)
	{
		_T *pThis = static_cast <_T *> ((expat_parser <_T> *) pUserData);
		pThis ->OnEndDoctypeDecl ();
	}

// @access Protected members
protected:

	XML_Parser		m_p;
};

//-----------------------------------------------------------------------------
//
// This is a test use for ExpatParser.
//
//-----------------------------------------------------------------------------

//class CExpat : public expatParser <CExpat>
//{
//// @access Constructors and destructors
//public:
//	
//	CExpat ()
//	{
//	}
//
//// @access Public handler methods
//public:
//
//	// @cmember Start element handler
//
//	virtual void OnStartElement (const XML_Char *pszName, 
//		const XML_Char **papszAttrs)
//	{
//		return;
//	}
//
//	// @cmember End element handler
//
//	virtual void OnEndElement (const XML_Char *pszName)
//	{
//		return;
//	}
//
//	// @cmember Character data handler
//
//	virtual void OnCharacterData (const XML_Char *pszData, int nLength)
//	{
//		return;
//	}
//
//	// @cmember Processing instruction handler
//
//	virtual void OnProcessingInstruction (const XML_Char *pszTarget, 
//		const XML_Char *pszData)
//	{
//		return;
//	}
//
//	// @cmember Comment handler
//
//	virtual void OnComment (const XML_Char *pszData)
//	{
//		return;
//	}
//
//	// @cmember Start CDATA section handler
//
//	virtual void OnStartCdataSection ()
//	{
//		return;
//	}
//
//	// @cmember End CDATA section handler
//
//	virtual void OnEndCdataSection ()
//	{
//		return;
//	}
//
//	// @cmember Default handler
//	
//	virtual void OnDefault (const XML_Char *pszData, int nLength)
//	{
//		return;
//	}
//	
//	// @cmember External entity ref handler
//	
//	virtual bool OnExternalEntityRef (const XML_Char *pszContext,
//		const XML_Char *pszBase, const XML_Char *pszSystemID,
//		const XML_Char *pszPublicID)
//	{
//		return false;
//	}
//	
//	// @cmember Unknown encoding handler
//	
//	virtual bool OnUnknownEncoding (const XML_Char *pszName, XML_Encoding *pInfo)
//	{
//		return false;
//	}
//	
//	// @cmember Start namespace declaration handler
//	
//	virtual void OnStartNamespaceDecl (const XML_Char *pszPrefix, 
//		const XML_Char *pszURI)
//	{
//		return;
//	}
//	
//	// @cmember End namespace declaration handler
//	
//	virtual void OnEndNamespaceDecl (const XML_Char *pszPrefix)
//	{
//		return;
//	}
//	
//	// @cmember XML declaration handler
//
//	virtual void OnXmlDecl (const XML_Char *pszVersion, 
//		const XML_Char *pszEncoding, bool fStandalone)
//	{
//		return;
//	}
//
//	// @cmember Start DOCTYPE declaration handler
//
//	virtual void OnStartDoctypeDecl (const XML_Char *pszDoctypeName, 
//		const XML_Char *pszSysID, const XML_Char *pszPubID,
//		bool fHasInternalSubset)
//	{
//		return;
//	}
//
//	// @cmember End DOCTYPE declaration handler
//
//	virtual void OnEndDoctypeDecl ()
//	{
//		return;
//	}
//};
}//namespace XML

}//namespace Xunlei
#endif // _EXPAT_PARSER_HPP_
