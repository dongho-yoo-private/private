/**
 * Copyright 2014 Applibot Inc.
 * \author tanaka tomonori
 * \file main_loop.cc
 * \ingroup application_group
 * \~english
 * \~japanese
 *  \brief
 */

#include <core/module/intern/cocos/main_loop.h>

#include <core/module/entrypoint.h>

MainLoop::MainLoop() {}
MainLoop::~MainLoop() {}

void MainLoop::update(float dt) { ADK_Main(); }

