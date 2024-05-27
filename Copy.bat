// 명령어			옵션		원본 파일이 있는 위치					사본 파일을 저장할 위치

xcopy			/y		.\Engine\Public\*.*					.\EngineSDK\Inc\
xcopy			/y/s		.\Engine\Public\*.*					.\EngineSDK\Inc\

xcopy			/y		.\Engine\Bin\Engine.dll				.\Client\bin\
xcopy			/y		.\Engine\Bin\Engine.dll				.\Tool\bin\
xcopy			/y		.\Engine\Bin\Engine.dll				.\EffectTool\bin\
xcopy			/y		.\Engine\Bin\Engine.lib				.\EngineSDK\Lib\

xcopy			/y		.\Engine\Bin\ShaderFiles\*.*				.\Client\bin\ShaderFiles\
xcopy			/y		.\Engine\Bin\ShaderFiles\*.*				.\Tool\bin\ShaderFiles\
xcopy			/y		.\Engine\Bin\ShaderFiles\*.*				.\EffectTool\bin\ShaderFiles\