/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   action.h for the Openbox window manager
   Copyright (c) 2007-2011   Dana Jansens

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   See the COPYING file for a copy of the GNU General Public License.
*/

#include "misc.h"
#include "frame.h"
#include "obt/xml.h"
#include "obt/keyboard.h"

#include <glib.h>
#include <X11/Xlib.h>

struct _ObActionList;

typedef struct _ObActionDefinition   ObActionDefinition;
typedef struct _ObAction             ObAction;
typedef struct _ObActionData         ObActionData;

typedef void     (*ObActionDataFreeFunc)(gpointer options);
typedef gboolean (*ObActionRunFunc)(ObActionData *data,
                                    gpointer options);
typedef gpointer (*ObActionDataSetupFunc)(GHashTable *config);
typedef void     (*ObActionShutdownFunc)(void);

/* functions for interactive actions */
/* return TRUE if the action is going to be interactive, or false to change
   your mind and make it not */
typedef gboolean (*ObActionIPreFunc)(guint initial_state, gpointer options);
typedef void     (*ObActionIPostFunc)(gpointer options);
typedef gboolean (*ObActionIInputFunc)(guint initial_state,
                                       XEvent *e,
                                       ObtIC *ic,
                                       gpointer options,
                                       gboolean *used);
typedef void     (*ObActionICancelFunc)(gpointer options);
typedef gpointer (*ObActionIDataSetupFunc)(GHashTable *config,
                                           ObActionIPreFunc *pre,
                                           ObActionIInputFunc *input,
                                           ObActionICancelFunc *cancel,
                                           ObActionIPostFunc *post);

struct _ObActionData {
    ObUserAction uact;
    guint state;
    gint x;
    gint y;
    gint button;

    struct _ObClient *client;
    ObFrameContext context;
};

void action_startup(gboolean reconfigure);
void action_shutdown(gboolean reconfigure);

/*! Use this if the actions created from this name may be interactive */
gboolean action_register_i(const gchar *name,
                           ObActionIDataSetupFunc setup,
                           ObActionDataFreeFunc free,
                           ObActionRunFunc run);

gboolean action_register(const gchar *name,
                         ObActionDataSetupFunc setup,
                         ObActionDataFreeFunc free,
                         ObActionRunFunc run);

gboolean action_set_shutdown(const gchar *name,
                             ObActionShutdownFunc shutdown);

gboolean action_is_interactive(ObAction *act);

/*! Create a new ObAction structure.
  @name The name of the action.
  @keys The names of the options passed to the action.
  @values The values of the options passed to the action, paired with the
    keys.  These are ObActionListValue objects.
*/
ObAction* action_new(const gchar *name, GHashTable *config);

void action_ref(ObAction *act);
void action_unref(ObAction *act);

/*! Runs a list of actions.
 @return TRUE if an interactive action was started, FALSE otherwise.
*/
gboolean action_run_acts(struct _ObActionList *acts,
                         ObUserAction uact,
                         guint state,
                         gint x,
                         gint y,
                         gint button,
                         ObFrameContext con,
                         struct _ObClient *client);

gboolean action_interactive_act_running(void);
void action_interactive_cancel_act(void);

gboolean action_interactive_input_event(XEvent *e);

/*! Function for actions to call when they are moving a client around */
void action_client_move(ObActionData *data, gboolean start);
