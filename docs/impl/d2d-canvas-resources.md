# D2D Canvas Resources

この文書は、このプロジェクト固有の実装メモである。D2D canvas resource の描画責務に関する判断をまとめる。

## Command List Canvas

`D2dCanvas` は `ID2D1CommandList` を保持する。

canvas に描画する場合は `pushCanvas` で device context の target を canvas command list に切り替え、`popCanvas` で元の target に戻す。このタイミングで canvas command list を `Close()` し、以後は `DrawImage` できる image resource として扱う。

## Canvas Draw

`Engine::draw(std::shared_ptr<Canvas>, opacity)` は `CanvasService::drawCanvas` に委譲する。

`drawCanvas` は `D2dCanvas` を `ID2D1CommandList` として `DrawImage` する。opacity が 1 未満の場合は一時 layer を push し、その layer 内で `DrawImage` する。

## Current Limitation

一度 `Close()` した command list は再び render target として使わない。現状の canvas は「描画して閉じ、再利用して描く」用途を想定する。再編集可能な canvas が必要になった場合は、command list の再作成や bitmap render target 方式を別途検討する。
