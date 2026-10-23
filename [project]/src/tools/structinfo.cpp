// Prints the in-memory layout of the 3D model structs. The .smd/.smb model
// files are raw dumps of these structs, so this tells us what the loader expects.
// Build: tools\structinfo.bat
#include <windows.h>
#include <stdio.h>
#include <stddef.h>
#include "smlib3d\\smd3d.h"
#include "smwsock.h"
#include "character.h"

#define O(f) printf("  %-14s %d\n", #f, (int)offsetof(smOBJ3D, f))

int main()
{
	printf("sizeof smOBJ3D %d\n", (int)sizeof(smOBJ3D));
	printf("sizeof smVERTEX %d smFACE %d smTEXLINK %d\n", (int)sizeof(smVERTEX), (int)sizeof(smFACE), (int)sizeof(smTEXLINK));
	printf("sizeof smTM_ROT %d smTM_POS %d smTM_SCALE %d smFMATRIX %d\n", (int)sizeof(smTM_ROT), (int)sizeof(smTM_POS), (int)sizeof(smTM_SCALE), (int)sizeof(smFMATRIX));
	printf("sizeof smDFILE_HEADER %d smDFILE_OBJINFO %d\n", (int)sizeof(smDFILE_HEADER), (int)sizeof(smDFILE_OBJINFO));
	printf("sizeof smMODELINFO %d  smMOTIONINFO %d  CHRMOTION_INFO_MAX %d\n", (int)sizeof(smMODELINFO), (int)sizeof(smMOTIONINFO), (int)MOTION_INFO_MAX);
	printf("sizeof smMATERIAL_GROUP %d  smMATERIAL %d\n", (int)sizeof(smMATERIAL_GROUP), (int)sizeof(smMATERIAL));
	printf("sizeof smSTAGE3D %d  smSTAGE_VERTEX %d  smSTAGE_FACE %d  smLIGHT3D %d\n", (int)sizeof(smSTAGE3D), (int)sizeof(smSTAGE_VERTEX), (int)sizeof(smSTAGE_FACE), (int)sizeof(smLIGHT3D));
	printf("smOBJ3D offsets:\n");
	O(nVertex); O(nFace); O(nTexLink); O(Physique); O(NodeName); O(TmRot); O(TmRotCnt); O(TmPosCnt); O(TmScaleCnt); O(TmFrameCnt);
#define OS(f) printf("  smSTAGE3D.%-12s %d\n", #f, (int)offsetof(smSTAGE3D, f))
	OS(nVertex); OS(nFace); OS(nTexLink); OS(nLight); OS(wAreaSize); OS(StageArea); OS(Vertex);
	printf("  MAP_SIZE %d\n", MAP_SIZE);
	printf("offsetof(smTEXTURE, Texture) %d  sizeof smTEXTUREHANDLE %d  MAX_TEXTURE %d\n", (int)offsetof(smTEXTURE, Texture), (int)sizeof(smTEXTUREHANDLE), (int)MAX_TEXTURE);
	return 0;
}
