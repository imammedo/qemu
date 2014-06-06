/*
 * memory hotplug test cases.
 *
 * Copyright (c) 2014 Red Hat Inc.
 *
 * Authors:
 *  Igor Mammedov <imammedo@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "libqtest.h"
#include "include/qemu/option.h"

typedef struct MOption {
    const char *size;
    const char *slots;
    const char *maxmem;
} MOption;

typedef struct TestData TestData;
struct TestData {
    const char *args;
    MOption m_option;
    void (*test)(const TestData *data);
};

static void test_machine(gconstpointer opaque)
{
    TestData *s = (TestData *)opaque;
    char *args = g_strdup_printf("%s", s->args);

    if (s->m_option.size) {
        char *old = args;
        args = g_strdup_printf("%s -m %s", args, s->m_option.size);
        g_free(old);
    }

    if (s->m_option.slots) {
        char *old = args;
        args = g_strdup_printf("%s -m slots=%s", args, s->m_option.slots);
        g_free(old);
    }

    if (s->m_option.maxmem) {
        char *old = args;
        args = g_strdup_printf("%s -m maxmem=%s", args, s->m_option.maxmem);
        g_free(old);
    }

    qtest_start(args);
    s->test(s);
    qtest_end();
    g_free(args);
}

#define DEFARGS "-net none -display none -machine accel=qtest "

#define ADD_COMMON(name, cmdline, sz, slots_nr, max_mem, func)               \
    {                                                                        \
        static const TestData d = {                                          \
        .args = DEFARGS cmdline,                                             \
        .m_option.size = sz,                                                 \
        .m_option.slots = slots_nr,                                          \
        .m_option.maxmem = max_mem,                                          \
        .test = func};                                                       \
        char *path;                                                          \
        path = g_strdup_printf("/memhp/%s/[%s %s%s%s%s%s%s]", name, cmdline, \
            d.m_option.size ? " -m size=" : "",                              \
            d.m_option.size ? d.m_option.size : "",                          \
            d.m_option.slots ? " -m slots=" : "",                            \
            d.m_option.slots ? d.m_option.slots : "",                        \
            d.m_option.maxmem ? " -m maxmem=" : "",                          \
            d.m_option.maxmem ? d.m_option.maxmem : "");                     \
        g_test_add_data_func(path, &d, test_machine);                        \
        g_free(path);                                                        \
    }

#define ADD_X86_COMMON(name, cmdline, sz, slots_nr, max_mem, func)           \
    if (strcmp(qtest_get_arch(), "i386") == 0 ||                             \
        strcmp(qtest_get_arch(), "x86_64") == 0) {                           \
        ADD_COMMON(name, cmdline, sz, slots_nr, max_mem, func)               \
    }
#define ADD_440FX_TEST(name, cmdline, sz, slots_nr, max_mem, func)           \
    ADD_X86_COMMON(name "/pc", cmdline "-M pc", sz, slots_nr, max_mem, func)

#define ADD_Q35_TEST(name, cmdline, sz, slots_nr, max_mem, func)             \
    ADD_X86_COMMON(name "/q35" , cmdline "-M q35", sz, slots_nr, max_mem, func)

#define ADD_TESTS(name, cmdline, sz, slots_nr, max_mem, func)                \
    {                                                                        \
        ADD_440FX_TEST(name, cmdline, sz, slots_nr, max_mem, func);          \
        ADD_Q35_TEST(name, cmdline, sz, slots_nr, max_mem, func);            \
    }

static void test_num_prop_value(const char *path, const char *prop,
                                const char *value)
{
    QDict *response;
    uint64_t ret, num_value;

    response = qmp("{ 'execute': 'qom-get',"
                   "  'arguments': { 'path': '%s',"
                   "                 'property': '%s' } }",
                   path, prop);
    /* qom-get may fail but should not, e.g., segfault. */
    g_assert(qdict_haskey(response, "return"));
    ret = qdict_get_int(response, "return");
    QDECREF(response);

    parse_option_size(prop, value, &num_value, &error_abort);
    g_assert(ret == num_value);
}

static void test_args(const TestData *data)
{
    if (data->m_option.size) {
        test_num_prop_value("/machine", "memory-size", data->m_option.size);
    }

    if (data->m_option.slots) {
        test_num_prop_value("/machine", "memory-slots", data->m_option.slots);
    }

    if (data->m_option.maxmem) {
        test_num_prop_value("/machine", "maxmem", data->m_option.maxmem);
    }
}

int main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);

    ADD_TESTS("args", "", "8M", NULL, NULL, test_args);
    ADD_TESTS("args", "", "8M", "1", "16M", test_args);
    ADD_TESTS("args", "", "8M", "256", "16M", test_args);
    ADD_TESTS("args", "", "8M", "1", "1G", test_args);
    ADD_TESTS("args", "", "8M", "1", "1T", test_args);

    return g_test_run();
}
