//#include "StdInc.h"
//#pragma once

struct Statement_s;
struct MenuEventHandlerSet;
struct ExpressionSupportingData;

struct ExpressionString
{
	const char *string;
};

union operandInternalDataUnion
{
	int intVal;
	float floatVal;
	ExpressionString *stringVal;
	Statement_s *function;
};

struct Operand
{
	int dataType;
	operandInternalDataUnion internals;
};

union entryInternalData
{
	int op;
	Operand operand;
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

struct expressionEntry
{
	int type;
	entryInternalData data;
};

struct Statement_s
{
	int numEntries;
	expressionEntry *entries;
	ExpressionSupportingData *supportingData;
	char unknown[0xC];
};

struct ItemFloatExpression
{
	int target;
	Statement_s *expression;
};

union EventData
{
	const char *unconditionalScript;
	ConditionalScript *conditionalScript;
	MenuEventHandlerSet *elseScript;
	SetLocalVarData *setLocalVarData;
};

#pragma pack(push, 4)
struct MenuEventHandler
{
	EventData eventData;
	char eventType;
};
#pragma pack(pop)

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

struct newsTickerDef_s
{
	int feedId;    // 0 = text input, 1 = motd
	int speed;
	int spacing;
	char unknown[0x10];
};

#define MAX_LB_COLUMNS 16

struct columnInfo_s
{
	int pos;
	int width;
	int maxChars;
	int alignment;
};

struct listBoxDef_s
{
	int startPos[4];
	int endPos[4];
	int drawPadding;
	float elementWidth;
	float elementHeight;
	int elementStyle;
	int numColumns;
	columnInfo_s columnInfo[MAX_LB_COLUMNS];
	MenuEventHandlerSet *doubleClick;
	int notselectable;
	int noscrollbars;
	int usepaging;
	vec4_t selectBorder;
	Material *selectIcon;
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

#define MAX_MULTI_DVARS 32

struct multiDef_s
{
	char *dvarList[MAX_MULTI_DVARS];
	char *dvarStr[MAX_MULTI_DVARS];
	float dvarValue[MAX_MULTI_DVARS];
	int count;
	int strDef;
};

struct textScrollDef_s
{
	int startTime;
};

union itemDefData_t
{
	listBoxDef_s *listBox;
	editFieldDef_s *editField;
	multiDef_s *multi;
	const char *enumDvarName;
	newsTickerDef_s *newsTicker;
	textScrollDef_s *textScroll;
	void *data;
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

struct windowDef_t
{
	const char *name;
	rectDef_s rect;
	rectDef_s rectClient;
	const char *group;
	int style;
	int border;
	int ownerDraw;
	int ownerDrawFlags;
	float borderSize;
	int staticFlags;
	int dynamicFlags[4];
	int nextTime;
	vec4_t foreColor;
	vec4_t backColor;
	vec4_t borderColor;
	vec4_t outlineColor;
	vec4_t disableColor;
	Material *background;
};

typedef windowDef_t Window;

struct itemDef_s
{
	windowDef_t window;
	rectDef_s textRect[4];
	int type;
	int dataType;
	int align;
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
	menuDef_t *parent;
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
	int dvarFlags;
	SoundAliasList *focusSound;
	float special;
	int cursorPos[4];
	itemDefData_t typeData;
	int imageTrack;
	int floatExpressionCount;
	ItemFloatExpression *floatExpressions;
	Statement_s *visibleExp;
	Statement_s *disabledExp;
	Statement_s *textExp;
	Statement_s *materialExp;
	vec4_t glowColor;
	bool decayActive;
	int fxBirthTime;
	int fxLetterTime;
	int fxDecayStartTime;
	int fxDecayDuration;
	int lastSoundPlayedTime;
};

struct StaticDvar
{
	dvar_t *dvar;
	const char *dvarName;
};

struct StaticDvarList
{
	int numStaticDvars;
	StaticDvar **staticDvars;
};

struct UIFunctionList
{
	int totalFunctions;
	Statement_s **functions;
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

enum transitionType
{
	TRANS_INACTIVE = 0x0,
	TRANS_LERP = 0x1,
};

enum triggerType
{
	TRIGGER_NONE = 0x0,
	TRIGGER_CLOSEMENU = 0x1,
};

struct menuTransition
{
	transitionType transitionType;
	int startTime;
	float startVal;
	float endVal;
	float time;
	triggerType endTriggerType;
};

struct menuDef_t
{
	Window window;
	const char *font;
	int fullscreen;
	int itemCount;
	int fontIndex;
	int cursorItems[4];
	int fadeCycle;
	float fadeClamp;
	float fadeAmount;
	float fadeInAmount;
	float blurRadius;
	MenuEventHandlerSet *onOpen;
	MenuEventHandlerSet *onRequestClose;
	MenuEventHandlerSet *onClose;
	MenuEventHandlerSet *onEsc;
	ItemKeyHandler *execKeys;
	Statement_s *visibleExp;
	const char *allowedBinding;
	const char *soundName;
	int imageTrack;
	vec4_t focusColor;
	Statement_s *rectXExp;
	Statement_s *rectYExp;
	Statement_s *rectHExp;
	Statement_s *rectWExp;
	itemDef_s **items;
	menuTransition scaleTransition[4];
	menuTransition alphaTransition[4];
	menuTransition xTransition[4];
	menuTransition yTransition[4];
	ExpressionSupportingData *expressionData;
};


struct MenuList
{
	const char *name;
	int menuCount;
	menuDef_t **menus;
};