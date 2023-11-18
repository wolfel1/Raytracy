## Draw Loop
    void Render:
        ClearViewport()

        for each mesh in scene_data 
            Bind Vertex Array
            Bind shader

            if mesh->IsIndexed()
                DrawIndexed
            else
                Draw