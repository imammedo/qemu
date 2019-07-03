/*
 * NUMA configuration test cases
 *
 * Copyright (c) 2017 Red Hat Inc.
 * Authors:
 *  Igor Mammedov <imammedo@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "libqtest.h"
#include "qapi/qmp/qdict.h"
#include "qapi/qmp/qlist.h"

typedef struct {
    const char *args;
    int ram_size; /* size in MB */
} TestData;

static char *make_cli(const TestData *data, int memdev_count,
                      const char *test_cli)
{
    int i;
    char *tmp = NULL;
    char *cli = g_strdup_printf("%s -m %d", data->args ? : "", data->ram_size);

    for (i = 0; memdev_count && (i < memdev_count); i++) {
        tmp = g_strdup_printf("%s -object memory-backend-ram,id=r%d,size=%dM",
                              cli, i, data->ram_size / memdev_count);
        g_free(cli);
        cli = tmp;
    }
    tmp = g_strdup_printf("%s %s", cli, test_cli);
    g_free(cli);
    cli = tmp;
    return cli;
}

static void test_mon_explicit(const void *data)
{
    char *s;
    char *cli;
    QTestState *qts;

    cli = make_cli(data, 2, "-smp 8 -numa node,nodeid=0,cpus=0-3,memdev=r0 "
                   "-numa node,nodeid=1,cpus=4-7,memdev=r1 ");
    qts = qtest_init(cli);

    s = qtest_hmp(qts, "info numa");
    g_assert(strstr(s, "node 0 cpus: 0 1 2 3"));
    g_assert(strstr(s, "node 1 cpus: 4 5 6 7"));
    g_free(s);

    qtest_quit(qts);
    g_free(cli);
}

static void test_mon_default(const void *data)
{
    char *s;
    char *cli;
    QTestState *qts;

    cli = make_cli(data, 2, "-smp 8 -numa node,memdev=r0 -numa node,memdev=r1");
    qts = qtest_init(cli);

    s = qtest_hmp(qts, "info numa");
    g_assert(strstr(s, "node 0 cpus: 0 2 4 6"));
    g_assert(strstr(s, "node 1 cpus: 1 3 5 7"));
    g_free(s);

    qtest_quit(qts);
    g_free(cli);
}

static void test_mon_partial(const void *data)
{
    char *s;
    char *cli;
    QTestState *qts;

    cli = make_cli(data, 2, "-smp 8 -numa node,nodeid=0,cpus=0-1,memdev=r0 "
                   "-numa node,nodeid=1,cpus=4-5,memdev=r1");
    qts = qtest_init(cli);

    s = qtest_hmp(qts, "info numa");
    g_assert(strstr(s, "node 0 cpus: 0 1 2 3 6 7"));
    g_assert(strstr(s, "node 1 cpus: 4 5"));
    g_free(s);

    qtest_quit(qts);
    g_free(cli);
}

static QList *get_cpus(QTestState *qts, QDict **resp)
{
    *resp = qtest_qmp(qts, "{ 'execute': 'query-cpus' }");
    g_assert(*resp);
    g_assert(qdict_haskey(*resp, "return"));
    return qdict_get_qlist(*resp, "return");
}

static void test_query_cpus(const void *data)
{
    char *cli;
    QDict *resp;
    QList *cpus;
    QObject *e;
    QTestState *qts;

    cli = make_cli(data, 2, "-smp 8 -numa node,cpus=0-3,memdev=r0 "
                   "-numa node,cpus=4-7,memdev=r1");
    qts = qtest_init(cli);
    cpus = get_cpus(qts, &resp);
    g_assert(cpus);

    while ((e = qlist_pop(cpus))) {
        QDict *cpu, *props;
        int64_t cpu_idx, node;

        cpu = qobject_to(QDict, e);
        g_assert(qdict_haskey(cpu, "CPU"));
        g_assert(qdict_haskey(cpu, "props"));

        cpu_idx = qdict_get_int(cpu, "CPU");
        props = qdict_get_qdict(cpu, "props");
        g_assert(qdict_haskey(props, "node-id"));
        node = qdict_get_int(props, "node-id");
        if (cpu_idx >= 0 && cpu_idx < 4) {
            g_assert_cmpint(node, ==, 0);
        } else {
            g_assert_cmpint(node, ==, 1);
        }
        qobject_unref(e);
    }

    qobject_unref(resp);
    qtest_quit(qts);
    g_free(cli);
}

static void pc_numa_cpu(const void *data)
{
    char *cli;
    QDict *resp;
    QList *cpus;
    QObject *e;
    QTestState *qts;

    cli = make_cli(data, 2, "-cpu pentium -smp 8,sockets=2,cores=2,threads=2 "
        "-numa node,nodeid=0,memdev=r0 -numa node,nodeid=1,memdev=r1 "
        "-numa cpu,node-id=1,socket-id=0 "
        "-numa cpu,node-id=0,socket-id=1,core-id=0 "
        "-numa cpu,node-id=0,socket-id=1,core-id=1,thread-id=0 "
        "-numa cpu,node-id=1,socket-id=1,core-id=1,thread-id=1");
    qts = qtest_init(cli);
    cpus = get_cpus(qts, &resp);
    g_assert(cpus);

    while ((e = qlist_pop(cpus))) {
        QDict *cpu, *props;
        int64_t socket, core, thread, node;

        cpu = qobject_to(QDict, e);
        g_assert(qdict_haskey(cpu, "props"));
        props = qdict_get_qdict(cpu, "props");

        g_assert(qdict_haskey(props, "node-id"));
        node = qdict_get_int(props, "node-id");
        g_assert(qdict_haskey(props, "socket-id"));
        socket = qdict_get_int(props, "socket-id");
        g_assert(qdict_haskey(props, "core-id"));
        core = qdict_get_int(props, "core-id");
        g_assert(qdict_haskey(props, "thread-id"));
        thread = qdict_get_int(props, "thread-id");

        if (socket == 0) {
            g_assert_cmpint(node, ==, 1);
        } else if (socket == 1 && core == 0) {
            g_assert_cmpint(node, ==, 0);
        } else if (socket == 1 && core == 1 && thread == 0) {
            g_assert_cmpint(node, ==, 0);
        } else if (socket == 1 && core == 1 && thread == 1) {
            g_assert_cmpint(node, ==, 1);
        } else {
            g_assert(false);
        }
        qobject_unref(e);
    }

    qobject_unref(resp);
    qtest_quit(qts);
    g_free(cli);
}

static void spapr_numa_cpu(const void *data)
{
    char *cli;
    QDict *resp;
    QList *cpus;
    QObject *e;
    QTestState *qts;

    cli = make_cli(data, 2, "-smp 4,cores=4 "
        "-numa node,nodeid=0,memdev=r0 -numa node,nodeid=1,memdev=r1 "
        "-numa cpu,node-id=0,core-id=0 "
        "-numa cpu,node-id=0,core-id=1 "
        "-numa cpu,node-id=0,core-id=2 "
        "-numa cpu,node-id=1,core-id=3");
    qts = qtest_init(cli);
    cpus = get_cpus(qts, &resp);
    g_assert(cpus);

    while ((e = qlist_pop(cpus))) {
        QDict *cpu, *props;
        int64_t core, node;

        cpu = qobject_to(QDict, e);
        g_assert(qdict_haskey(cpu, "props"));
        props = qdict_get_qdict(cpu, "props");

        g_assert(qdict_haskey(props, "node-id"));
        node = qdict_get_int(props, "node-id");
        g_assert(qdict_haskey(props, "core-id"));
        core = qdict_get_int(props, "core-id");

        if (core >= 0 && core < 3) {
            g_assert_cmpint(node, ==, 0);
        } else if (core == 3) {
            g_assert_cmpint(node, ==, 1);
        } else {
            g_assert(false);
        }
        qobject_unref(e);
    }

    qobject_unref(resp);
    qtest_quit(qts);
    g_free(cli);
}

static void aarch64_numa_cpu(const void *data)
{
    char *cli;
    QDict *resp;
    QList *cpus;
    QObject *e;
    QTestState *qts;

    cli = make_cli(data, 2, "-smp 2 "
        "-numa node,nodeid=0,memdev=r0 -numa node,nodeid=1,memdev=r1 "
        "-numa cpu,node-id=1,thread-id=0 "
        "-numa cpu,node-id=0,thread-id=1");
    qts = qtest_init(cli);
    cpus = get_cpus(qts, &resp);
    g_assert(cpus);

    while ((e = qlist_pop(cpus))) {
        QDict *cpu, *props;
        int64_t thread, node;

        cpu = qobject_to(QDict, e);
        g_assert(qdict_haskey(cpu, "props"));
        props = qdict_get_qdict(cpu, "props");

        g_assert(qdict_haskey(props, "node-id"));
        node = qdict_get_int(props, "node-id");
        g_assert(qdict_haskey(props, "thread-id"));
        thread = qdict_get_int(props, "thread-id");

        if (thread == 0) {
            g_assert_cmpint(node, ==, 1);
        } else if (thread == 1) {
            g_assert_cmpint(node, ==, 0);
        } else {
            g_assert(false);
        }
        qobject_unref(e);
    }

    qobject_unref(resp);
    qtest_quit(qts);
    g_free(cli);
}

static void pc_dynamic_cpu_cfg(const void *data)
{
    QObject *e;
    QDict *resp;
    QList *cpus;
    char *cli = make_cli(data, 2, "-nodefaults --preconfig -smp 2");
    QTestState *qs = qtest_init(cli);

    /* create 2 numa nodes */
    g_assert(!qmp_rsp_is_err(qtest_qmp(qs, "{ 'execute': 'set-numa-node',"
        " 'arguments': { 'type': 'node', 'nodeid': 0, 'memdev': 'r0' } }")));
    g_assert(!qmp_rsp_is_err(qtest_qmp(qs, "{ 'execute': 'set-numa-node',"
        " 'arguments': { 'type': 'node', 'nodeid': 1, 'memdev': 'r1' } }")));

    /* map 2 cpus in non default reverse order
     * i.e socket1->node0, socket0->node1
     */
    g_assert(!qmp_rsp_is_err(qtest_qmp(qs, "{ 'execute': 'set-numa-node',"
        " 'arguments': { 'type': 'cpu', 'node-id': 0, 'socket-id': 1 } }")));
    g_assert(!qmp_rsp_is_err(qtest_qmp(qs, "{ 'execute': 'set-numa-node',"
        " 'arguments': { 'type': 'cpu', 'node-id': 1, 'socket-id': 0 } }")));

    /* let machine initialization to complete and run */
    g_assert(!qmp_rsp_is_err(qtest_qmp(qs, "{ 'execute': 'x-exit-preconfig' }")));
    qtest_qmp_eventwait(qs, "RESUME");

    /* check that CPUs are mapped as expected */
    resp = qtest_qmp(qs, "{ 'execute': 'query-hotpluggable-cpus'}");
    g_assert(qdict_haskey(resp, "return"));
    cpus = qdict_get_qlist(resp, "return");
    g_assert(cpus);
    while ((e = qlist_pop(cpus))) {
        const QDict *cpu, *props;
        int64_t socket, node;

        cpu = qobject_to(QDict, e);
        g_assert(qdict_haskey(cpu, "props"));
        props = qdict_get_qdict(cpu, "props");

        g_assert(qdict_haskey(props, "node-id"));
        node = qdict_get_int(props, "node-id");
        g_assert(qdict_haskey(props, "socket-id"));
        socket = qdict_get_int(props, "socket-id");

        if (socket == 0) {
            g_assert_cmpint(node, ==, 1);
        } else if (socket == 1) {
            g_assert_cmpint(node, ==, 0);
        } else {
            g_assert(false);
        }
        qobject_unref(e);
    }
    qobject_unref(resp);

    qtest_quit(qs);
    g_free(cli);
}

int main(int argc, char **argv)
{
    const char *arch = qtest_get_arch();
    TestData data = { .ram_size = 128 };

    if (!strcmp(arch, "aarch64")) {
        data.args = "-machine virt";
    } else if (!strcmp(arch, "ppc64")) {
        data.ram_size = 512;
    }

    g_test_init(&argc, &argv, NULL);

    qtest_add_data_func("/numa/mon/default", &data, test_mon_default);
    qtest_add_data_func("/numa/mon/cpus/explicit", &data, test_mon_explicit);
    qtest_add_data_func("/numa/mon/cpus/partial", &data, test_mon_partial);
    qtest_add_data_func("/numa/qmp/cpus/query-cpus", &data, test_query_cpus);

    if (!strcmp(arch, "i386") || !strcmp(arch, "x86_64")) {
        qtest_add_data_func("/numa/pc/cpu/explicit", &data, pc_numa_cpu);
        qtest_add_data_func("/numa/pc/dynamic/cpu", &data, pc_dynamic_cpu_cfg);
    }

    if (!strcmp(arch, "ppc64")) {
        qtest_add_data_func("/numa/spapr/cpu/explicit", &data, spapr_numa_cpu);
    }

    if (!strcmp(arch, "aarch64")) {
        qtest_add_data_func("/numa/aarch64/cpu/explicit", &data,
                            aarch64_numa_cpu);
    }

    return g_test_run();
}
