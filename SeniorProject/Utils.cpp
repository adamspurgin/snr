#include "stdafx.h"
#include "Utils.h"
#include "Sphere.h"
#include "Light.h"
#include "Camera.h"
#include <xmllite.h>
#include <MsXml6.h>
#include <Windows.h>
#include <objbase.h>
#include <string>
#include <mbstring.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <comutil.h>

#ifndef DEBUG_ON
#define DEBUG_ON 0
#endif

using namespace std;

#define interp(a, b, c)  (( (b) * (c) + (a) * ( 1 - (c)))

int line = 0;

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)){ line=__LINE__; goto CleanUp; }} while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

#define DEBUG(s) do{ if(DEBUG_ON){std::cout << s}}while(0)

// Helper function to create a VT_BSTR variant from a null terminated string. 
HRESULT VariantFromString(PCWSTR wszValue, VARIANT &Variant)
{
    HRESULT hr = S_OK;
    BSTR bstr = SysAllocString(wszValue);
    CHK_ALLOC(bstr);

    V_VT(&Variant)   = VT_BSTR;
    V_BSTR(&Variant) = bstr;

CleanUp:
    return hr;
}

// Helper function to create a DOM instance. 
HRESULT CreateAndInitDOM(IXMLDOMDocument **ppDoc)
{
    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
    if (SUCCEEDED(hr))
    {
        // these methods should not fail so don't inspect result
        (*ppDoc)->put_async(VARIANT_FALSE);  
        (*ppDoc)->put_validateOnParse(VARIANT_FALSE);
        (*ppDoc)->put_resolveExternals(VARIANT_FALSE);
        (*ppDoc)->put_preserveWhiteSpace(VARIANT_TRUE);
    }
    return hr;
}

// Helper function to display parse error.
// It returns error code of the parse error.
HRESULT ReportParseError(IXMLDOMDocument *pDoc, char *szDesc)
{
    HRESULT hr = S_OK;
    HRESULT hrRet = E_FAIL; // Default error code if failed to get from parse error.
    IXMLDOMParseError *pXMLErr = NULL;
    BSTR bstrReason = NULL;

    CHK_HR(pDoc->get_parseError(&pXMLErr));
    CHK_HR(pXMLErr->get_errorCode(&hrRet));
    CHK_HR(pXMLErr->get_reason(&bstrReason));
    printf("%s\n%S\n", szDesc, bstrReason);

CleanUp:
    SAFE_RELEASE(pXMLErr);
    SysFreeString(bstrReason);
    return hrRet;
}

Utils::Utils(void)
{
}


Utils::~Utils(void)
{
}

/*IXMLDOMNode* getElement(int i, IXMLDOMDocument* doc){
	stringstream querry;
	querry << "c4d_file/v6_basedocument/v6_root_object/v6_rootlist2d/obj_pluginobject[" << i << "]";
	IXMLDOMNode *pNode = NULL;
	_bstr_t qString(querry.str);
	CHK_HR(pXMLDom->selectSingleNode(qString, &pNode));
}*/

void util::match(std::istream& str, const char* string){
	char buffer[100];
	int length = strlen(string);
	str.read(buffer, length);
	buffer[length] = 0;
	if(strcmp(buffer, string) != 0){
		cerr << "MISMATCH";
		cerr << string << " : " << buffer;
		exit(1);
	}
}

void util::eat(std::istream& str){
	char* bin;
	while(isspace(str.peek())){
		str.read(bin, 1);
	}
}

std::istream* util::stripSpaces(std::istream& str){
	stringstream *ss = new stringstream;
	char c;
	while(str.get(c)){
		if(c == EOF){
			break;
		} else if(isspace(c)) {
			continue;
		} else {
			ss->write(&c, 1);
		}
	}
	return ss;
}

enum objType{SPHERE, CAMERA, LIGHT, ERR};

objType getType(IXMLDOMNode* node){
	HRESULT hr = S_OK;
	IXMLDOMNode *id = NULL;
	CHK_HR(node->selectSingleNode(L"chunk[@id='110064']/string", &id));
	IXMLDOMNamedNodeMap *attrs;
	IXMLDOMNode *item;
	CHK_HR(id->get_attributes(&attrs));
	CHK_HR(attrs->getNamedItem(L"v", &item));
	VARIANT val;
	CHK_HR(item->get_nodeValue(&val));
	
	if(!wcscmp(val.bstrVal, L"Camera")){
		return CAMERA;
	} else if (!wcscmp(val.bstrVal, L"Sphere")){
		return SPHERE;
	} else if (!wcscmp(val.bstrVal, L"Light")){
		return LIGHT;
	} else {
		printf("--%S--\n", val.bstrVal);
		return ERR;
	}
CleanUp:
	printf("cleanup\n");
	return ERR;
}

Sphere* getSphere(IXMLDOMNode* node){
	HRESULT hr = S_OK;
	IXMLDOMNode *position = NULL, *radius = NULL;
	IXMLDOMNamedNodeMap *attrs;
	IXMLDOMNode *x, *y, *z, *rad;
	VARIANT val;
	Sphere *s = new Sphere();

	CHK_HR(node->selectSingleNode(L"v6_baseobject[@level='4']/vector[1]", &position));
	
	CHK_HR(position->get_attributes(&attrs));

	CHK_HR(attrs->getNamedItem(L"x", &x));
	CHK_HR(x->get_nodeValue(&val));
	s->center.x = wcstod(val.bstrVal, NULL);

	CHK_HR(attrs->getNamedItem(L"y", &y));
	CHK_HR(y->get_nodeValue(&val));
	s->center.y = wcstod(val.bstrVal, NULL);

	CHK_HR(attrs->getNamedItem(L"z", &z));
	CHK_HR(z->get_nodeValue(&val));	
	s->center.z = wcstod(val.bstrVal, NULL);

	CHK_HR(node->selectSingleNode(L"v6_baselist2d/container/data[@id='1110']/real", &radius));
	CHK_HR(radius->get_attributes(&attrs));
	CHK_HR(attrs->getNamedItem(L"v", &rad));
	CHK_HR(rad->get_nodeValue(&val));
	s->radius = wcstod(val.bstrVal, NULL);
	SAFE_RELEASE(position);
	SAFE_RELEASE(radius);
	SAFE_RELEASE(attrs);
	SAFE_RELEASE(x);
	SAFE_RELEASE(z);
	SAFE_RELEASE(x);
	return s;

CleanUp:
	SAFE_RELEASE(position);
	SAFE_RELEASE(radius);
	SAFE_RELEASE(attrs);
	SAFE_RELEASE(x);
	SAFE_RELEASE(z);
	SAFE_RELEASE(x);
	return NULL;
}

Light* getLight(IXMLDOMNode* node){
	HRESULT hr = S_OK;
	IXMLDOMNode *position = NULL;
	IXMLDOMNamedNodeMap *attrs;
	IXMLDOMNode *x = NULL, *y = NULL, *z = NULL;
	VARIANT val;
	Light *l = new Light();

	CHK_HR(node->selectSingleNode(L"v6_baseobject[@level='4']/vector[1]", &position));

	CHK_HR(position->get_attributes(&attrs));

	CHK_HR(attrs->getNamedItem(L"x", &x));
	CHK_HR(x->get_nodeValue(&val));
	l->position.x = wcstod(val.bstrVal, NULL);

	CHK_HR(attrs->getNamedItem(L"y", &y));
	CHK_HR(y->get_nodeValue(&val));
	l->position.y = wcstod(val.bstrVal, NULL);

	CHK_HR(attrs->getNamedItem(L"z", &z));
	CHK_HR(z->get_nodeValue(&val));	
	l->position.z = wcstod(val.bstrVal, NULL);
	SAFE_RELEASE(position);
	SAFE_RELEASE(attrs);
	SAFE_RELEASE(x);
	SAFE_RELEASE(z);
	SAFE_RELEASE(x);
	return l;

CleanUp:
	cerr << "ERROR: " << line << "\n";
	SAFE_RELEASE(position);
	SAFE_RELEASE(attrs);
	SAFE_RELEASE(x);
	SAFE_RELEASE(z);
	SAFE_RELEASE(x);
	return NULL;
}

Camera* getCamera(IXMLDOMNode* node){
	HRESULT hr = S_OK;
	IXMLDOMNode *position = NULL, *focalLength = NULL, *focalWidth = NULL, *test = NULL;
	IXMLDOMNamedNodeMap *attrs;
	IXMLDOMNode *x = NULL, *y = NULL, *z = NULL, *length = NULL, *width = NULL;

	VARIANT val;
	Camera *c = new Camera();

	//get position
	CHK_HR(node->selectSingleNode(L"v6_baseobject[@level='4']/vector[1]", &position));

	position->get_attributes(&attrs);

	CHK_HR(attrs->getNamedItem(L"x", &x));
	CHK_HR(x->get_nodeValue(&val));
	c->position.x = wcstod(val.bstrVal, NULL);

	CHK_HR(attrs->getNamedItem(L"y", &y));
	CHK_HR(y->get_nodeValue(&val));
	c->position.y = wcstod(val.bstrVal, NULL);

	CHK_HR(attrs->getNamedItem(L"z", &z));
	CHK_HR(z->get_nodeValue(&val));	
	c->position.z = wcstod(val.bstrVal, NULL);

	//get rotation
	CHK_HR(node->selectSingleNode(L"v6_baseobject[@level='4']/vector[3]", &position));

	position->get_attributes(&attrs);

	CHK_HR(attrs->getNamedItem(L"x", &x));
	CHK_HR(x->get_nodeValue(&val));
	c->rotation.x = wcstod(val.bstrVal, NULL);

	CHK_HR(attrs->getNamedItem(L"y", &y));
	CHK_HR(y->get_nodeValue(&val));
	c->rotation.y = wcstod(val.bstrVal, NULL);

	CHK_HR(attrs->getNamedItem(L"z", &z));
	CHK_HR(z->get_nodeValue(&val));	
	c->rotation.z = wcstod(val.bstrVal, NULL);

	//get focal length
	CHK_HR(node->selectSingleNode(L"v6_baselist2d[@level='6']/container[@id='-1']/data[@id='500']/real", &focalLength));

	CHK_HR(focalLength->get_attributes(&attrs));

	CHK_HR(attrs->getNamedItem(L"v", &length));
	CHK_HR(length->get_nodeValue(&val));
	c->focalLength= wcstod(val.bstrVal, NULL);

	//get focal width
	CHK_HR(node->selectSingleNode(L"v6_baselist2d[@level='6']/container[@id='-1']/data[@id='1006']/real", &focalWidth));
	CHK_HR(focalWidth->get_attributes(&attrs));

	CHK_HR(attrs->getNamedItem(L"v", &width));
	CHK_HR(width->get_nodeValue(&val));
	c->focalWidth= wcstod(val.bstrVal, NULL);

	SAFE_RELEASE(position);
	SAFE_RELEASE(focalLength);
	SAFE_RELEASE(focalWidth);
	SAFE_RELEASE(attrs);
	SAFE_RELEASE(x);
	SAFE_RELEASE(z);
	SAFE_RELEASE(x);
	SAFE_RELEASE(length);
	SAFE_RELEASE(width);
	return c;
CleanUp:
	cerr << "Cleanup: " << line << "\n";
	SAFE_RELEASE(position);
	SAFE_RELEASE(focalLength);
	SAFE_RELEASE(focalWidth);
	SAFE_RELEASE(attrs);
	SAFE_RELEASE(x);
	SAFE_RELEASE(z);
	SAFE_RELEASE(x);
	SAFE_RELEASE(length);
	SAFE_RELEASE(width);
	return NULL;
}


Scene* util::getFromXml(const char* path){
	line= __LINE__;
	HRESULT hr = S_OK;
    IXMLDOMDocument *pXMLDom=NULL;
    IXMLDOMParseError *pXMLErr = NULL;
	IXMLDOMNode *pNode = NULL;
	IXMLDOMNodeList *pNodes = NULL;

    BSTR bstrXML = NULL;
    BSTR bstrErr = NULL;
	BSTR querry;
	BSTR bstrNodeName;

    VARIANT_BOOL varStatus;
    VARIANT varFileName;

	Scene *scene = new Scene();
    VariantInit(&varFileName);

    CHK_HR(CreateAndInitDOM(&pXMLDom));    

    // XML file name to load
	BSTR filepath = SysAllocStringLen(NULL, strlen(path)); 
	mbstowcs(filepath, path, strlen(path)); 

	//BSTR filepath = BSTR(path);
	CHK_HR(VariantFromString(filepath, varFileName));
    CHK_HR(pXMLDom->load(varFileName, &varStatus));
    if (varStatus == VARIANT_TRUE)
    {
        CHK_HR(pXMLDom->get_xml(&bstrXML));
        //printf("XML DOM loaded from scn.xml:\n%S\n", bstrXML);
    }
    else
    {
        // Failed to load xml, get last parsing error
        CHK_HR(pXMLDom->get_parseError(&pXMLErr));
        CHK_HR(pXMLErr->get_reason(&bstrErr));
        printf("Failed to load DOM from XML file. %S\n", bstrErr);
    }
	BSTR bstrQuery1 = SysAllocString(L"c4d_file/v6_basedocument/v6_root_object/v6_rootlist2d/obj_pluginobject");
    CHK_ALLOC(bstrQuery1);
    CHK_HR(pXMLDom->selectNodes(bstrQuery1, &pNodes));
    if (pNodes)
    {
		long length;
		pNodes->get_length(&length);
		//printf("there are %d elements here\n", length);
		for(int i=0; i<length; i++){
			IXMLDOMNode *current = NULL;
			Sphere *s;
			Camera *c;
			Light *l;
			CHK_HR(pNodes->get_item(i, &current));
			switch(getType(current)){
			case SPHERE:
				//printf("got a sphere\n");
				s = getSphere(current);
				scene->addElement(s);
				break;
			case LIGHT:
				//printf("got a light\n");
				l = getLight(current);
				scene->addElement(l);
				break;
			case CAMERA:
				//printf("got a camera\n");
				c = getCamera(current);
				scene->addElement(c);
				break;
			default:
				cerr << "ERROR\n";
			}
		}
		//scene->toStream(std::cout);
    }
    else
    {
        CHK_HR(ReportParseError(pXMLDom, "Error while calling selectSingleNode."));
    }

	BSTR bstrQueryResX = SysAllocString(L"//data[@id='5008']/long");//L"c4d_file/v6_basedocument/v6_root_viewpanel/v6_rootlist2d/v6_viewpanel/v6_root_basedraw/v6_rootlist2d/v6_basedraw/v6_basedrawprefs/real[3]");
	BSTR bstrQueryResY = SysAllocString(L"//data[@id='5009']/long");//L"c4d_file/v6_basedocument/v6_root_viewpanel/v6_rootlist2d/v6_viewpanel/v6_root_basedraw/v6_rootlist2d/v6_basedraw/v6_basedrawprefs/real[4]");
    CHK_ALLOC(bstrQueryResX);
	CHK_ALLOC(bstrQueryResY);
	IXMLDOMNode *resx = NULL, *resy = NULL;
	IXMLDOMNamedNodeMap *vals;
    CHK_HR(pXMLDom->selectSingleNode(bstrQueryResX, &resx));
	CHK_HR(pXMLDom->selectSingleNode(bstrQueryResY, &resy));

	BSTR resxXml, resyXml;

	resx->get_xml(&resxXml);
	resy->get_xml(&resyXml);

	cout<< resxXml << "\n";
	cout<< resyXml << "\n";
	
	CHK_HR(resx->get_attributes(&vals));
	vals->getNamedItem(L"v", &resx);

	CHK_HR(resy->get_attributes(&vals));
	vals->getNamedItem(L"v", &resy);

	VARIANT val;
	resx->get_nodeValue(&val);
	scene->width = wcstol(val.bstrVal, NULL, 10);

	resy->get_nodeValue(&val);
	scene->height = wcstol(val.bstrVal, NULL, 10);

	return scene;

CleanUp:
	cout << "ERROR: " << line << "\n";
    SAFE_RELEASE(pXMLDom);
    SAFE_RELEASE(pXMLErr);
    SysFreeString(bstrXML);
    SysFreeString(bstrErr);
    VariantClear(&varFileName);
	return scene;
}

bool util::quadRoots(double a, double b, double c, double *r1, double *r2){
	if(b*b - 4*a*c < 0){
		return false;
	} else {
		*r1 = (-b + sqrt(b*b - 4*a*c)) / (2*a);
		*r2 = (-b - sqrt(b*b - 4*a*c)) / (2*a);
		return true;
	}
}

int util::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

void util::saveBitmap(BSTR path, Bitmap* image){
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	CLSID   encoderClsid;
	GetEncoderClsid(L"image/bmp", &encoderClsid);
	image->Save(path, &encoderClsid, NULL);
}

