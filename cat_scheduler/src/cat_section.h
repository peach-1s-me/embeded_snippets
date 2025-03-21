
#ifndef CAT_SECTION_H
#define CAT_SECTION_H

/**
 * 如果使用gnu工具，则需要在链接脚本中添加以下信息
 *
 * PROVIDE_HIDDEN (name_start = .);
 * KEEP (*(name))
 * PROVIDE_HIDDEN (name_end = .);
 * 
 * 
 */

#if defined(__CC_ARM) || (defined ( __ARMCC_VERSION ) && ( __ARMCC_VERSION >= 6010050 ))
    #define IMPORT_SECTION(x) \
        extern cat_uint32_t x##$$Base; \
        extern cat_uint32_t x##$$Limit;

    #define SECTION_START(x)  (&x##$$Base)
    #define SECTION_END(x)    (&x##$$Limit)
    #define SECTION_SIZE(x)   ((cat_uint32_t)&x##$$Limit - (cat_uint32_t)&x##$$Base)

    #define SECTION(x)  __attribute__((used, section(x)))
#elif defined(__GNUC__)
    /* 将在链接脚本中指定的段引入到源文件中，表现为两个变量：段起始地址和段结束地址 */
    #define IMPORT_SECTION(x) \
        extern unsigned short x##_start; \
        extern unsigned short x##_end;

    /* 获取段信息 */
    #define SECTION_START(x)  (&x##_start)
    #define SECTION_END(x)    (&x##_end)
    #define SECTION_SIZE(x)   ((unsigned short)&x##$$Limit - (unsigned short)&x##$$Base)

    /* 声明将被修饰的数据放入指定段中 */
    #define SECTION(x)  __attribute__((section(x)))
#else
    #error "no match compiler"
#endif

#endif /* #ifndef CAT_SECTION_H */