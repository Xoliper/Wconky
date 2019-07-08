#ifndef EXTENSION_MANAGEICONS_DLL_H
#define EXTENSION_MANAGEICONS_DLL_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILDING_EXTENSION_DLL
#define EXTENSION_DLL __declspec(dllexport)
#else
#define EXTENSION_DLL __declspec(dllimport)
#endif

#ifdef __cplusplus
}
#endif


class EXTENSION_DLL ManageIcons
{
public:
	ManageIcons();
    virtual ~ManageIcons();
    
	void Toggle(void);
	
private:
	bool state;
};


#endif  