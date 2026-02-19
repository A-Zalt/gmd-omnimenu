.method public native nativeOnFileSaveChosen(Ljava/lang/String;)V
.end method

.method public native nativeOnFileOpenChosen(Ljava/lang/String;)V
.end method

.method public showSaveFilePicker(Ljava/lang/String;)V
    .registers 5

    new-instance v0, Landroid/content/Intent;
    const-string v1, "android.intent.action.CREATE_DOCUMENT"
    invoke-direct {v0, v1}, Landroid/content/Intent;-><init>(Ljava/lang/String;)V

    const-string v1, "*/*"
    invoke-virtual {v0, v1}, Landroid/content/Intent;->setType(Ljava/lang/String;)Landroid/content/Intent;

    const-string v1, "android.intent.extra.TITLE"
    invoke-virtual {v0, v1, p1}, Landroid/content/Intent;->putExtra(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;

    const/16 v1, 0x3E9  # 1001
    invoke-virtual {p0, v0, v1}, Lorg/cocos2dx/lib/Cocos2dxActivity;->startActivityForResult(Landroid/content/Intent;I)V

    return-void
.end method

.method public showOpenFilePicker()V
    .registers 5

    new-instance v0, Landroid/content/Intent;
    const-string v1, "android.intent.action.OPEN_DOCUMENT"
    invoke-direct {v0, v1}, Landroid/content/Intent;-><init>(Ljava/lang/String;)V

    const-string v1, "*/*"
    invoke-virtual {v0, v1}, Landroid/content/Intent;->setType(Ljava/lang/String;)Landroid/content/Intent;

    const/16 v1, 0x3EA  # 1002
    invoke-virtual {p0, v0, v1}, Lorg/cocos2dx/lib/Cocos2dxActivity;->startActivityForResult(Landroid/content/Intent;I)V

    return-void
.end method

.method protected onActivityResult(IILandroid/content/Intent;)V
    .locals 4

    invoke-super {p0, p1, p2, p3}, Lorg/cocos2dx/lib/Cocos2dxActivity;->onActivityResult(IILandroid/content/Intent;)V

    # --- Handle CREATE_DOCUMENT (Save) ---
    const/16 v0, 0x3E9              # 1001
    if-ne p1, v0, :check_open

    const/4 v0, -1                  # RESULT_OK
    if-ne p2, v0, :end

    # Get data URI
    invoke-virtual {p3}, Landroid/content/Intent;->getData()Landroid/net/Uri;
    move-result-object v1

    # Null check
    if-eqz v1, :end

    # Convert to String
    invoke-virtual {v1}, Landroid/net/Uri;->toString()Ljava/lang/String;
    move-result-object v2

    # Call native method (instance call)
    invoke-virtual {p0, v2}, Lcom/robtopx/geometryjump/GeometryJump;->nativeOnFileSaveChosen(Ljava/lang/String;)V

    goto :end


    # --- Handle OPEN_DOCUMENT (Open) ---
    :check_open
    const/16 v0, 0x3EA              # 1002
    if-ne p1, v0, :end

    const/4 v0, -1                  # RESULT_OK
    if-ne p2, v0, :end

    # Get data URI
    invoke-virtual {p3}, Landroid/content/Intent;->getData()Landroid/net/Uri;
    move-result-object v1

    if-eqz v1, :end

    # Convert to String
    invoke-virtual {v1}, Landroid/net/Uri;->toString()Ljava/lang/String;
    move-result-object v2

    # Call native method (instance call)
    invoke-virtual {p0, v2}, Lcom/robtopx/geometryjump/GeometryJump;->nativeOnFileOpenChosen(Ljava/lang/String;)V


    :end
    return-void
.end method

.method public static getInstance()Lcom/robtopx/geometryjump/GeometryJump;
    .locals 1

    # return instance
    sget-object v0, Lcom/robtopx/geometryjump/GeometryJump;->instance:Lcom/robtopx/geometryjump/GeometryJump;
    return-object v0
.end method