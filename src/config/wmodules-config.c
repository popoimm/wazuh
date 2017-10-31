/*
 * Wazuh Module Configuration
 * Copyright (C) 2016 Wazuh Inc.
 * April 25, 2016.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#include "wazuh_modules/wmodules.h"

static const char *XML_NAME = "name";

// Read wodle element

int Read_WModule(const OS_XML *xml, xml_node *node, void *d1, __attribute__((unused)) void *d2)
{
    wmodule **wmodules = (wmodule**)d1;
    wmodule *cur_wmodule;
    xml_node **children = NULL;

    if (!node->attributes[0]) {
        merror("No such attribute '%s' at module.", XML_NAME);
        return OS_INVALID;
    }

    if (strcmp(node->attributes[0], XML_NAME)) {
        merror("Module attribute is not '%s'.", XML_NAME);
        return OS_INVALID;
    }

    // Allocate memory

    if ((cur_wmodule = *wmodules)) {
        while (cur_wmodule->next)
            cur_wmodule = cur_wmodule->next;

        os_calloc(1, sizeof(wmodule), cur_wmodule->next);
        cur_wmodule = cur_wmodule->next;
    } else
        *wmodules = cur_wmodule = calloc(1, sizeof(wmodule));

    if (!cur_wmodule) {
        merror(MEM_ERROR, errno, strerror(errno));
        return (OS_INVALID);
    }

    // Get children

    if (!(children = OS_GetElementsbyNode(xml, node))) {
        merror(XML_INVELEM, node->element);
        return OS_INVALID;
    }

    // Select module by name

    if (!strcmp(node->values[0], WM_OSCAP_CONTEXT.name)){
        if (wm_oscap_read(xml, children, cur_wmodule) < 0) {
            OS_ClearNode(children);
            return OS_INVALID;
        }
    } else if (!strcmp(node->values[0], WM_COMMAND_CONTEXT.name)){
        if (wm_command_read(children, cur_wmodule) < 0) {
            OS_ClearNode(children);
            return OS_INVALID;
        }
    } else {
        merror("Unknown module '%s'", node->values[0]);
    }

    OS_ClearNode(children);
    return 0;
}

int Test_WModule(const char * path) {
    int fail = 0;
    wmodule *test_wmodule;
    os_calloc(1, sizeof(wmodule), test_wmodule);

    if (ReadConfig(CAGENT_CONFIG | CWMODULE, path, &test_wmodule, NULL) < 0) {
        merror(RCONFIG_ERROR,"WModule", path);
        fail = 1;
    }

    wm_free(test_wmodule);

    if (fail) {
        return -1;
    } else {
        return 0;
    }
}
