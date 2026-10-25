
#define DEFINE_HOOKEDVAR(type, name, addr) \
	inline type _##name##() { return *(type*)GetVarAddr((void*)addr); }  \
	inline type* _rawaddr_##name##() { return (type*)GetVarAddr((void*)addr); }  \
	inline void _set_##name##(type val) { memcpy(GetVarAddr((void*)addr), (void*)&val, sizeof(type)); }

#define DEFINE_HOOKEDARRAYVAR(type, name, addr) \
	inline type* _##name##() { return (type*)GetVarAddr((void*)addr); } \
	inline void _set_##name##(type val) { memcpy(GetVarAddr((void*)addr), (void*)&val, sizeof(type)); }


void HookFunc(void* funcaddr, void* ourfunc);
void* GetVarAddr(void* varaddr);
void HookingInit(void* baseprocaddr);