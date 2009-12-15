/* -*- indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*-

   screen.h for the Openbox window manager
   Copyright (c) 2003-2007   Dana Jansens

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

#ifndef __screen_h
#define __screen_h

#include "misc.h"
#include "geom.h"

struct _ObClient;

#define DESKTOP_ALL (0xffffffff)

/*! The number of available desktops */
extern guint screen_num_desktops;
/*! The number of virtual "xinerama" screens/heads */
extern guint screen_num_monitors;
/*! The current desktop */
extern guint screen_desktop;
/*! The desktop which was last visible */
extern guint screen_last_desktop;
/*! Are we in showing-desktop mode? */
extern gboolean screen_showing_desktop;
/*! The support window also used for focus and stacking */
extern Window screen_support_win;
/*! The last time at which the user changed desktops */
extern Time screen_desktop_user_time;

typedef struct ObDesktopLayout {
    ObOrientation orientation;
    ObCorner start_corner;
    guint rows;
    guint columns;
} ObDesktopLayout;
extern ObDesktopLayout screen_desktop_layout;

/*! An array of gchar*'s which are desktop names in UTF-8 format */
extern gchar **screen_desktop_names;

/*! Take over the screen, set the basic hints on it claming it as ours */
gboolean screen_annex();

/*! Once the screen is ours, set up its initial state */
void screen_startup(gboolean reconfig);
/*! Free resources */
void screen_shutdown(gboolean reconfig);

/*! Figure out the new size of the screen and adjust stuff for it */
void screen_resize();

/*! Change the number of available desktops */
void screen_set_num_desktops(guint num);
/*! Change the current desktop */
void screen_set_desktop(guint num, gboolean dofocus);
/*! Add a new desktop either at the end or inserted at the current desktop */
void screen_add_desktop(gboolean current);
/*! Remove a desktop, either at the end or the current desktop */
void screen_remove_desktop(gboolean current);

guint screen_find_desktop(guint from, ObDirection dir,
                          gboolean wrap, gboolean linear);

/*! Show the desktop popup/notification */
void screen_show_desktop_popup(guint d);
/*! Hide it */
void screen_hide_desktop_popup();

/*! Shows and focuses the desktop and hides all the client windows, or
  returns to the normal state, showing client windows.
  @param If show_only is non-NULL, then only that client is shown (assuming
         show is FALSE (restoring from show-desktop mode), and the rest are
         iconified.
*/
void screen_show_desktop(gboolean show, struct _ObClient *show_only);

/*! Updates the desktop layout from the root property if available */
void screen_update_layout();

/*! Get desktop names from the root window property */
void screen_update_desktop_names();

/*! Installs or uninstalls a colormap for a client. If client is NULL, then
  it handles the root colormap. */
void screen_install_colormap(struct _ObClient *client, gboolean install);

void screen_update_areas();

Rect *screen_physical_area_all_monitors();

Rect *screen_physical_area_monitor(guint head);

/*! Returns the monitor which contains the active window, or the one
  containing the pointer otherwise. */
guint screen_monitor_active(void);

Rect *screen_physical_area_active(void);

/*! Returns the primary monitor, as specified by the config.
  @fixed If TRUE, then this will always return a fixed monitor, otherwise
         it may change based on where focus is, or other heuristics.
 */
guint screen_monitor_primary(gboolean fixed);

/*! Returns physical area for the primary monitor, as specified by the config.
  @fixed If TRUE, then this will always use a fixed monitor as primary,
         otherwise it may change based on where focus is, or other heuristics.
         See screen_monitor_primary().
*/
Rect *screen_physical_area_primary(gboolean fixed);

/* doesn't include struts which the search area is already outside of when
   'search' is not NULL */
#define SCREEN_AREA_ALL_MONITORS ((unsigned)-1)
#define SCREEN_AREA_ONE_MONITOR  ((unsigned)-2)

/*! @param head is the number of the head or one of SCREEN_AREA_ALL_MONITORS,
           SCREEN_AREA_ONE_MONITOR
    @param search NULL or the whole monitor(s)
 */
Rect* screen_area(guint desktop, guint head, Rect *search);

gboolean screen_physical_area_monitor_contains(guint head, Rect *search);

/*! Determines which physical monitor a rectangle is on by calculating the
    area of the part of the rectable on each monitor.  The number of the
    monitor containing the greatest area of the rectangle is returned.
*/
guint screen_find_monitor(Rect *search);

/*! Finds the monitor which contains the point @x, @y */
guint screen_find_monitor_point(guint x, guint y);

/*! Sets the root cursor. This function decides which cursor to use, but you
  gotta call it to let it know it should change. */
void screen_set_root_cursor();

/*! Gives back the pointer's position in x and y. Returns TRUE if the pointer
  is on this screen and FALSE if it is on another screen. */
gboolean screen_pointer_pos(gint *x, gint *y);

/*! Returns the monitor which contains the pointer device */
guint screen_monitor_pointer(void);

#endif
