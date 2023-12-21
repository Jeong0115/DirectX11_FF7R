xcopy /s /y  /exclude:exclude_list.txt ".\Project\Engine\*.h" ".\External\include\Engine"
xcopy /s /y  /exclude:exclude_list.txt ".\Project\Engine\*.hlsl" ".\OutputFile\bin\content\shader"
xcopy /s /y  /exclude:exclude_list.txt ".\Project\Engine\*.hlsli" ".\OutputFile\bin\content\shader"