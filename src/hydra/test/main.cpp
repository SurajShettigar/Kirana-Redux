// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#include <renderer_plugin.hpp>
#include <render_delegate.hpp>

#include <pxr/pxr.h>
#include <pxr/base/gf/matrix4f.h>
#include <pxr/imaging/hd/unitTestDelegate.h>
// #include <pxr/imaging/hdx/renderTask.h>
// #include <pxr/imaging/hd/engine.h>

int main(int argc, char **argv)
{
    using namespace pxr;

    HdKiranaRendererPlugin *plugin = new HdKiranaRendererPlugin();

    HdRenderDelegate *render_delegate = plugin->CreateRenderDelegate();
    HdRenderIndex *render_index = HdRenderIndex::New(render_delegate, {});

    HdUnitTestDelegate *scene_delegate = new HdUnitTestDelegate(render_index, SdfPath::AbsoluteRootPath());

    scene_delegate->AddCube(SdfPath("/Cube1"), GfMatrix4f(1.0));

    // SdfPath render_task("/renderTask");
    // scene_delegate->AddTask<HdxRenderTask>(render_task);
    // scene_delegate->UpdateTask(render_task, HdTokens->params, VtValue(HdxRenderTaskParams()));
    // scene_delegate->UpdateTask(render_task, HdTokens->collection,
    //                            VtValue(HdRprimCollection(HdTokens->geometry, HdReprSelector(HdReprTokens->refined))));
    //
    // // Ask Hydra to execute our render task.
    // HdEngine engine;
    // HdTaskSharedPtrVector tasks = {render_index->GetTask(render_task)};
    // engine.Execute(render_index, &tasks);

    delete scene_delegate;
    delete render_index;
    delete render_delegate;
    delete plugin;
    return 0;
}