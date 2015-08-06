//#include "StdInc.h"
//#pragma once

typedef float vec_t;
typedef vec_t vec4_t[4];
struct expression_s;
struct statement_s;
struct menuDef_t;
enum operationEnum;

struct keyname_t
{
	const char *name;
	int keynum;
};

struct ItemFloatExpressionEntry
{
	int target;
	const char *s1;
	const char *s2;
};

ItemFloatExpressionEntry itemFloatExpressions[19] =
{
	{ 0, "rect", "x" },
	{ 1, "rect", "y" },
	{ 2, "rect", "w" },
	{ 3, "rect", "h" },
	{ 4, "forecolor", "r" },
	{ 5, "forecolor", "g" },
	{ 6, "forecolor", "b" },
	{ 7, "forecolor", "rgb" },
	{ 8, "forecolor", "a" },
	{ 9, "glowcolor", "r" },
	{ 10, "glowcolor", "g" },
	{ 11, "glowcolor", "b" },
	{ 12, "glowcolor", "rgb" },
	{ 13, "glowcolor", "a" },
	{ 14, "backcolor", "r" },
	{ 15, "backcolor", "g" },
	{ 16, "backcolor", "b" },
	{ 17, "backcolor", "rgb" },
	{ 18, "backcolor", "a" }
};

#define ITEM_TYPE_TEXT				0		// simple text
#define ITEM_TYPE_BUTTON			1		// button, basically text with a border
#define ITEM_TYPE_RADIOBUTTON		2		// toggle button, may be grouped
#define ITEM_TYPE_CHECKBOX			3		// check box
#define ITEM_TYPE_EDITFIELD 		4		// editable text, associated with a dvar
#define ITEM_TYPE_COMBO 			5		// drop down list
#define ITEM_TYPE_LISTBOX			6		// scrollable list
#define ITEM_TYPE_MODEL 			7		// model
#define ITEM_TYPE_OWNERDRAW 		8		// owner draw, name specs what it is
#define ITEM_TYPE_NUMERICFIELD		9		// editable text, associated with a dvar
#define ITEM_TYPE_SLIDER			10		// mouse speed, volume, etc.
#define ITEM_TYPE_YESNO 			11		// yes no dvar setting
#define ITEM_TYPE_MULTI 			12		// multiple list setting, enumerated
#define ITEM_TYPE_DVARENUM 			13		// multiple list setting, enumerated from a dvar
#define ITEM_TYPE_BIND				14		// bind
#define ITEM_TYPE_MENUMODEL 		15		// special menu model
#define ITEM_TYPE_VALIDFILEFIELD	16		// text must be valid for use in a dos filename
#define ITEM_TYPE_DECIMALFIELD		17		// editable text, associated with a dvar, which allows decimal input
#define ITEM_TYPE_UPREDITFIELD		18		// editable text, associated with a dvar
#define ITEM_TYPE_GAME_MESSAGE_WINDOW 19	// game message window
#define ITEM_TYPE_NEWSTICKER		20		// horizontal scrollbox
#define ITEM_TYPE_TEXTSCROLL		21		// vertical scrollbox
#define ITEM_TYPE_EMAILFIELD		22
#define ITEM_TYPE_PASSWORDFIELD		23

struct MenuEventHandlerSet;
struct Statement_s;

struct UIFunctionList
{
	int totalFunctions;
	Statement_s **functions;
};

struct StaticDvar
{
	dvar_t *dvar;
	char *dvarName;
};

struct StaticDvarList
{
	int numStaticDvars;
	StaticDvar **staticDvars;
};

struct StringList
{
	int totalStrings;
	const char **strings;
};

struct ExpressionSupportingData
{
	UIFunctionList uifunctions;
	StaticDvarList staticDvarList;
	StringList uiStrings;
};

enum expDataType : int
{
	VAL_INT = 0x0,
	VAL_FLOAT = 0x1,
	VAL_STRING = 0x2,
	VAL_FUNCTION = 0x3,
};

struct ExpressionString
{
	const char *string;
};

union operandInternalDataUnion
{
	int intVal;
	float floatVal;
	ExpressionString stringVal;
	Statement_s *function;
};

struct Operand
{
	expDataType dataType;
	operandInternalDataUnion internals;
};

union entryInternalData
{
	//operationEnum op;
	Operand operand;
};

/* expressionEntry->type */
#define OPERATOR	0
#define OPERAND		1

struct expressionEntry	// 0xC
{
	int type;
	entryInternalData data;
};

struct Statement_s	// 0x18
{
	int numEntries;
	expressionEntry *entries;
	ExpressionSupportingData *supportingData;
	char unknown[0xC];	// padding er wut?
};

struct SetLocalVarData
{
	const char *localVarName;
	Statement_s *expression;
};

struct ConditionalScript
{
	MenuEventHandlerSet *eventHandlerSet;
	Statement_s *eventExpression;  // loads this first
};

union EventData
{
	const char *unconditionalScript;
	ConditionalScript *conditionalScript;
	MenuEventHandlerSet *elseScript;
	SetLocalVarData *setLocalVarData;
};

enum EventType
{
	EVENT_UNCONDITIONAL = 0x0,
	EVENT_IF = 0x1,
	EVENT_ELSE = 0x2,
	EVENT_SET_LOCAL_VAR_BOOL = 0x3,
	EVENT_SET_LOCAL_VAR_INT = 0x4,
	EVENT_SET_LOCAL_VAR_FLOAT = 0x5,
	EVENT_SET_LOCAL_VAR_STRING = 0x6,
	EVENT_COUNT = 0x7,
};

struct MenuEventHandler
{
	EventData eventData;
	EventType eventType;
};

struct MenuEventHandlerSet
{
	int eventHandlerCount;
	MenuEventHandler **eventHandlers;
};

struct ItemKeyHandler
{
	int key;
	MenuEventHandlerSet *action;
	ItemKeyHandler *next;
};

#pragma pack(push, 4)
struct rectDef_s
{
	float x;
	float y;
	float w;
	float h;
	char horzAlign;
	char vertAlign;
};
#pragma pack(pop)

/* windowDef_t->dynamicFlags */
// 0x1
#define WINDOWDYNAMIC_HASFOCUS		0x00000002
#define WINDOWDYNAMIC_VISIBLE		0x00000004
#define WINDOWDYNAMIC_FADEOUT		0x00000010
#define WINDOWDYNAMIC_FADEIN		0x00000020
// 0x40
// 0x80
#define WINDOWDYNAMIC_CLOSED		0x00000800
// 0x2000
#define WINDOWDYNAMIC_BACKCOLOR		0x00008000
#define WINDOWDYNAMIC_FORECOLOR		0x00010000

/* windowDef_t->staticFlags */
#define WINDOWSTATIC_DECORATION				0x00100000
#define WINDOWSTATIC_HORIZONTALSCROLL			0x00200000
#define WINDOWSTATIC_SCREENSPACE				0x00400000
#define WINDOWSTATIC_AUTOWRAPPED				0x00800000
#define WINDOWSTATIC_POPUP						0x01000000
#define WINDOWSTATIC_OUTOFBOUNDSCLICK			0x02000000
#define WINDOWSTATIC_LEGACYSPLITSCREENSCALE	0x04000000
#define WINDOWSTATIC_HIDDENDURINGFLASH			0x10000000
#define WINDOWSTATIC_HIDDENDURINGSCOPE			0x20000000
#define WINDOWSTATIC_HIDDENDURINGUI			0x40000000
#define WINDOWSTATIC_TEXTONLYFOCUS				0x80000000

struct windowDef_t // 0xA4
{
	const char *name;	// 0x00
	rectDef_s rect;
	rectDef_s rectClient;
	char *group;		// 0x2C
	int style;			// 0x30
	int border;			// 0x34
	int ownerDraw;		// 0x38
	int ownerDrawFlags;	// 0x3C
	float borderSize;	// 0x40
	int staticFlags;	// 0x44
	int dynamicFlags;	// 0x48
	int nextTime;		// 0x4C
	float foreColor[4];	// 0x50
	float backColor[4];	// 0x60
	float borderColor[4];// 0x70
	float outlineColor[4];// 0x80
	float disableColor[4];// 0x90
	Material *background;	// 0xA0
};

enum ItemFloatExpressionTarget
{
	ITEM_FLOATEXP_TGT_RECT_X = 0x0,
	ITEM_FLOATEXP_TGT_RECT_Y = 0x1,
	ITEM_FLOATEXP_TGT_RECT_W = 0x2,
	ITEM_FLOATEXP_TGT_RECT_H = 0x3,
	ITEM_FLOATEXP_TGT_FORECOLOR_R = 0x4,
	ITEM_FLOATEXP_TGT_FORECOLOR_G = 0x5,
	ITEM_FLOATEXP_TGT_FORECOLOR_B = 0x6,
	ITEM_FLOATEXP_TGT_FORECOLOR_RGB = 0x7,
	ITEM_FLOATEXP_TGT_FORECOLOR_A = 0x8,
	ITEM_FLOATEXP_TGT_GLOWCOLOR_R = 0x9,
	ITEM_FLOATEXP_TGT_GLOWCOLOR_G = 0xA,
	ITEM_FLOATEXP_TGT_GLOWCOLOR_B = 0xB,
	ITEM_FLOATEXP_TGT_GLOWCOLOR_RGB = 0xC,
	ITEM_FLOATEXP_TGT_GLOWCOLOR_A = 0xD,
	ITEM_FLOATEXP_TGT_BACKCOLOR_R = 0xE,
	ITEM_FLOATEXP_TGT_BACKCOLOR_G = 0xF,
	ITEM_FLOATEXP_TGT_BACKCOLOR_B = 0x10,
	ITEM_FLOATEXP_TGT_BACKCOLOR_RGB = 0x11,
	ITEM_FLOATEXP_TGT_BACKCOLOR_A = 0x12,
	ITEM_FLOATEXP_TGT__COUNT = 0x13,
};

struct ItemFloatExpression
{
	ItemFloatExpressionTarget target;
	Statement_s *expression;
};

struct editFieldDef_s
{
	float minVal;
	float maxVal;
	float defVal;
	float range;
	int maxChars;
	int maxCharsGotoNext;
	int maxPaintChars;
	int paintOffset;
};

struct multiDef_s	// 0x188
{
	const char *dvarList[32];
	const char *dvarStr[32];
	float dvarValue[32];
	int count;
	int strDef;
};

struct columnInfo_s
{
	int xpos;
	int width;
	int maxChars;
	int alignment;
};

struct listBoxDef_s	// 0x144
{
	// somethings not right here
	int startPos[2];
	int endPos[2];
	float elementWidth;
	float elementHeight;
	int elementStyle;
	int numColumns;
	columnInfo_s columnInfo[16];
	MenuEventHandlerSet *doubleClick;	// 0xC8
	int notselectable;
	int noscrollbars;
	int usepaging;
	float selectBorder[4];
	Material *selectIcon;
};

struct newsTickerDef_s
{
	int feedId;
	int speed;
	int spacing;
};

struct textScrollDef_s
{
	int startTime;
};

union itemDefData_t
{
	listBoxDef_s *listBox;
	editFieldDef_s *editField;
	newsTickerDef_s *ticker;
	multiDef_s *multiDef;
	const char *enumDvarName;
	textScrollDef_s *scroll;
	void *data;
};

struct itemDef_t
{
	windowDef_t window;
	rectDef_s textRect;
	int type;
	int dataType;
	int alignment;
	int fontEnum;
	int textAlignMode;
	float textAlignX;
	float textAlignY;
	float textScale;
	int textStyle;
	int gameMsgWindowIndex;
	int gameMsgWindowMode;
	const char *text;
	int textSaveGameInfo;
	int parent;
	MenuEventHandlerSet *mouseEnterText;
	MenuEventHandlerSet *mouseExitText;
	MenuEventHandlerSet *mouseEnter;
	MenuEventHandlerSet *mouseExit;
	MenuEventHandlerSet *action;
	MenuEventHandlerSet *accept;
	MenuEventHandlerSet *onFocus;
	MenuEventHandlerSet *leaveFocus;
	const char *dvar;
	const char *dvarTest;
	ItemKeyHandler *onKey;
	const char *enableDvar;
	const char *localVar;
	int dvarFlags;
	const char *focusSound;
	float special;
	int cursorPos;
	itemDefData_t typeData;
	int imageTrack;
	int floatExpressionCount;
	ItemFloatExpression *floatExpressions;
	Statement_s *visibleExp;
	Statement_s *disabledExp;
	Statement_s *textExp;
	Statement_s *materialExp;
	float glowColor[4];
	bool decayActive;
	int fxBirthTime;
	int fxLetterTime;
	int fxDecayStartTime;
	int fxDecayDuration;
	int lastSoundPlayedTime;
};

struct menuTransition	// 0x18
{
	int transitionType;
	int startTime;
	float startVal;
	float endVal;
	float time;
	int endTriggerType;
};

struct menuDef_t
{
	windowDef_t window;
	int font;
	int fullscreen;
	int itemCount;
	int fontIndex;
	int cursorItems;
	int fadeCycle;
	float fadeClamp;
	float fadeAmount;
	float fadeInAmount;
	float blurRadius;
	MenuEventHandlerSet *onOpen;
	MenuEventHandlerSet *onRequestClose;
	MenuEventHandlerSet *onClose;
	MenuEventHandlerSet *onEsc;
	ItemKeyHandler *onKey;
	Statement_s *visibleExp;
	const char *allowedBinding;
	const char *soundLoop;
	int imageTrack;
	float focusColor[4];
	Statement_s *rectXExp;
	Statement_s *rectYExp;
	Statement_s *rectHExp;
	Statement_s *rectWExp;
	Statement_s *openSoundExp;
	Statement_s *closeSoundExp;
	itemDef_t **items;
	char unknown[112];
	ExpressionSupportingData *expressionData;
};

struct MenuList
{
	const char *name;
	int menuCount;
	menuDef_t **menus;
};
