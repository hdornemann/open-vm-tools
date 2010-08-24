/*********************************************************
 * Copyright (C) 2007 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation version 2.1 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the Lesser GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 *********************************************************/

/**
 * @file unityPlatform.h
 *
 * Implementation specific functionality
 */

#ifndef _UNITY_PLATFORM_H_
#define _UNITY_PLATFORM_H_

#include "rpcout.h"
#include "unityWindowTracker.h"
#include "unity.h"

/**
 * Container used to store and send Unity updates.
 */

typedef struct {
   DynBuf updates;      ///< See @ref vmtools_unity_uwtGuestRpc.
   size_t cmdSize;      /**< @brief Size of RpcOut command prefix.
                             Used as a convenient offset within #updates when
                             resetting the update buffer. */
   RpcOut *rpcOut;
} UnityUpdateChannel;

typedef struct {
   int x;
   int y;
   int width;
   int height;
} UnityRect;

typedef struct _UnityPlatform UnityPlatform;

/*
 * Implemented by unityPlatform[Win32|X11|Cocoa (ha!)].c
 */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
Bool UnityPlatformIsSupported(void);
UnityPlatform *UnityPlatformInit(UnityWindowTracker *tracker,
                                 UnityUpdateChannel *updateChannel,
                                 int *blockedWnd);
void UnityPlatformCleanup(UnityPlatform *up);
void UnityPlatformRegisterCaps(UnityPlatform *up);
void UnityPlatformUnregisterCaps(UnityPlatform *up);
Bool UnityPlatformUpdateWindowState(UnityPlatform *up,
                                    UnityWindowTracker *tracker);
void UnityPlatformSaveSystemSettings(UnityPlatform *up);
void UnityPlatformRestoreSystemSettings(UnityPlatform *up);
Bool UnityPlatformGetWindowPath(UnityPlatform *up,
                                UnityWindowId window,
                                DynBuf *windowPathUtf8,
                                DynBuf *execPathUtf8);
Bool UnityPlatformGetNativeWindowPath(UnityPlatform *up,
                                      UnityWindowId window,
                                      DynBuf *buf);
Bool UnityPlatformGetBinaryInfo(UnityPlatform *up,
                                const char *pathUtf8,
                                DynBuf *buf);
Bool UnityPlatformSetTopWindowGroup(UnityPlatform *up,
                                    UnityWindowId *windows,
                                    unsigned int windowCount);
Bool UnityPlatformCloseWindow(UnityPlatform *up, UnityWindowId window);
Bool UnityPlatformShowWindow(UnityPlatform *up, UnityWindowId window);
Bool UnityPlatformHideWindow(UnityPlatform *up, UnityWindowId window);
Bool UnityPlatformMinimizeWindow(UnityPlatform *up, UnityWindowId window);
Bool UnityPlatformUnminimizeWindow(UnityPlatform *up, UnityWindowId window);
Bool UnityPlatformMaximizeWindow(UnityPlatform *up, UnityWindowId window);
Bool UnityPlatformUnmaximizeWindow(UnityPlatform *up, UnityWindowId window);
Bool UnityPlatformGetWindowContents(UnityPlatform *up,
                                    UnityWindowId window,
                                    DynBuf *imageData,
                                    uint32 *width,
                                    uint32 *height);
Bool UnityPlatformMoveResizeWindow(UnityPlatform *up,
                                   UnityWindowId window,
                                   UnityRect *moveResizeRect);
void UnityPlatformShowTaskbar(UnityPlatform *up, Bool showTaskbar);
Bool UnityPlatformGetIconData(UnityPlatform *up,
                              UnityWindowId window,
                              UnityIconType iconType,
                              UnityIconSize iconSize,
                              uint32 dataOffset,
                              uint32 dataLength,
                              DynBuf *imageData,
                              uint32 *fullLength);
Bool UnityPlatformSetDesktopWorkAreas(UnityPlatform *up,
                                      UnityRect workAreas[],
                                      uint32 numWorkAreas);
Bool UnityPlatformSetDesktopConfig(UnityPlatform *up,
                                   const UnityVirtualDesktopArray *desktops);
Bool UnityPlatformSetInitialDesktop(UnityPlatform *up,
                                    UnityDesktopId desktopId);
Bool UnityPlatformSetDesktopActive(UnityPlatform *up,
                                   UnityDesktopId desktopId);
Bool UnityPlatformSetWindowDesktop(UnityPlatform *up,
                                   UnityWindowId windowId,
                                   UnityDesktopId desktopId);
Bool UnityPlatformStickWindow(UnityPlatform *up,
                              UnityWindowId windowId);
Bool UnityPlatformUnstickWindow(UnityPlatform *up,
                                UnityWindowId windowId);
void UnityPlatformSetInterlockMinimizeOperation(UnityPlatform *up,Bool enabled);
Bool UnityPlatformConfirmMinimizeOperation(UnityPlatform *up,
                                           UnityWindowId windowId,
                                           uint32 sequence,
                                           Bool allow);
Bool UnityPlatformIsUnityRunning(UnityPlatform *up);
Bool UnityPlatformStartHelperThreads(UnityPlatform *up);
void UnityPlatformKillHelperThreads(UnityPlatform *up);
void UnityPlatformUnlock(UnityPlatform *up);
void UnityPlatformLock(UnityPlatform *up);
void UnityPlatformUpdateDnDDetWnd(UnityPlatform *up,
                                  Bool show);
void UnityPlatformSetActiveDnDDetWnd(UnityPlatform *up, UnityDnD *detWnd);

void UnityPlatformDoUpdate(UnityPlatform *up, Bool incremental);

void UnityPlatformSetConfigDesktopColor(UnityPlatform *up, int desktopColor);

Bool UnityPlatformRequestWindowContents(UnityPlatform *up,
                                        UnityWindowId windowIds[],
                                        uint32 numWindowIds);

Bool UnityPlatformSendMouseWheel(UnityPlatform *up,
                                 int32 deltaX,
                                 int32 deltaY,
                                 int32 deltaZ,
                                 uint32 modifierFlags);

/*
 * Function called by UnityUpdateCallbackFn whenever a window is removed from
 * the tracker.
 *
 * NOTE: This function is called with the platform lock held.
 */
void UnityPlatformWillRemoveWindow(UnityPlatform *up, UnityWindowId windowId);

/* Functions implemented in unity.c for use by the platform-specific code. */
void UnityGetUpdateCommon(int flags, DynBuf *buf);
Bool UnityUpdateChannelInit(UnityUpdateChannel *updateChannel);
void UnityUpdateChannelCleanup(UnityUpdateChannel *updateChannel);
Bool UnitySendUpdates(UnityUpdateChannel *updateChannel);
Bool UnitySendRequestMinimizeOperation(UnityWindowId windowId, uint32 sequence);

/* Sends the provided window contents to the host. */
Bool UnitySendWindowContents(UnityWindowId windowID,
                             uint32 imageWidth,
                             uint32 imageHeight,
                             const char *imageData,
                             uint32 imageLength);

void UnityPlatformSetDisableCompositing(UnityPlatform *up, Bool disabled);

#ifdef __cplusplus
};
#endif // __cplusplus

#endif
