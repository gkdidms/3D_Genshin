// ��ɾ�			�ɼ�		���� ������ �ִ� ��ġ					�纻 ������ ������ ��ġ

xcopy			/y		.\Engine\Public\*.*					.\EngineSDK\Inc\
xcopy			/y/s		.\Engine\Public\*.*					.\EngineSDK\Inc\

xcopy			/y		.\Engine\Bin\Engine.dll				.\Client\bin\
xcopy			/y		.\Engine\Bin\Engine.dll				.\Tool\bin\
xcopy			/y		.\Engine\Bin\Engine.dll				.\EffectTool\bin\
xcopy			/y		.\Engine\Bin\Engine.lib				.\EngineSDK\Lib\

xcopy			/y		.\Engine\Bin\ShaderFiles\*.*				.\Client\bin\ShaderFiles\
xcopy			/y		.\Engine\Bin\ShaderFiles\*.*				.\Tool\bin\ShaderFiles\
xcopy			/y		.\Engine\Bin\ShaderFiles\*.*				.\EffectTool\bin\ShaderFiles\