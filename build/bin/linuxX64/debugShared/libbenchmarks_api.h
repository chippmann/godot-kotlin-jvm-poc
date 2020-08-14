#ifndef KONAN_LIBBENCHMARKS_H
#define KONAN_LIBBENCHMARKS_H
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
typedef bool            libbenchmarks_KBoolean;
#else
typedef _Bool           libbenchmarks_KBoolean;
#endif
typedef unsigned short     libbenchmarks_KChar;
typedef signed char        libbenchmarks_KByte;
typedef short              libbenchmarks_KShort;
typedef int                libbenchmarks_KInt;
typedef long long          libbenchmarks_KLong;
typedef unsigned char      libbenchmarks_KUByte;
typedef unsigned short     libbenchmarks_KUShort;
typedef unsigned int       libbenchmarks_KUInt;
typedef unsigned long long libbenchmarks_KULong;
typedef float              libbenchmarks_KFloat;
typedef double             libbenchmarks_KDouble;
typedef float __attribute__ ((__vector_size__ (16))) libbenchmarks_KVector128;
typedef void*              libbenchmarks_KNativePtr;
struct libbenchmarks_KType;
typedef struct libbenchmarks_KType libbenchmarks_KType;

typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Byte;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Short;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Int;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Long;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Float;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Double;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Char;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Boolean;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_kotlin_Unit;
typedef struct {
  libbenchmarks_KNativePtr pinned;
} libbenchmarks_kref_Simple;

extern void godot_gdnative_init(void* options);
extern void godot_gdnative_terminate(void* options);
extern void godot_nativescript_init(void* handle);
extern void godot_nativescript_terminate(void* handle);

typedef struct {
  /* Service functions. */
  void (*DisposeStablePointer)(libbenchmarks_KNativePtr ptr);
  void (*DisposeString)(const char* string);
  libbenchmarks_KBoolean (*IsInstance)(libbenchmarks_KNativePtr ref, const libbenchmarks_KType* type);
  libbenchmarks_kref_kotlin_Byte (*createNullableByte)(libbenchmarks_KByte);
  libbenchmarks_kref_kotlin_Short (*createNullableShort)(libbenchmarks_KShort);
  libbenchmarks_kref_kotlin_Int (*createNullableInt)(libbenchmarks_KInt);
  libbenchmarks_kref_kotlin_Long (*createNullableLong)(libbenchmarks_KLong);
  libbenchmarks_kref_kotlin_Float (*createNullableFloat)(libbenchmarks_KFloat);
  libbenchmarks_kref_kotlin_Double (*createNullableDouble)(libbenchmarks_KDouble);
  libbenchmarks_kref_kotlin_Char (*createNullableChar)(libbenchmarks_KChar);
  libbenchmarks_kref_kotlin_Boolean (*createNullableBoolean)(libbenchmarks_KBoolean);
  libbenchmarks_kref_kotlin_Unit (*createNullableUnit)(void);

  /* User functions. */
  struct {
    struct {
      struct {
        libbenchmarks_KType* (*_type)(void);
        libbenchmarks_kref_Simple (*Simple)();
        libbenchmarks_KInt (*benchmarkAvg)(libbenchmarks_kref_Simple thiz);
        libbenchmarks_KInt (*benchmarkSimpleAdd)(libbenchmarks_kref_Simple thiz);
      } Simple;
      struct {
        void (*GDNativeInit)(void* options);
        void (*GDNativeTerminate)(void* options);
        void (*NativeScriptInit)(void* handle);
        void (*NativeScriptTerminate)(void* handle);
      } godot;
    } root;
  } kotlin;
} libbenchmarks_ExportedSymbols;
extern libbenchmarks_ExportedSymbols* libbenchmarks_symbols(void);
#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif  /* KONAN_LIBBENCHMARKS_H */
