using UnityEngine;
using UnityEditor;
using UnityEditor.AssetImporters;

[ScriptedImporter(3, "melo")]
public sealed class MeloImporter : ScriptedImporter
{
    const string IconPath = "Packages/com.melowrite.audio/Editor/MeloFileIcon.png";

    public override void OnImportAsset(AssetImportContext ctx)
    {
        var file = ScriptableObject.CreateInstance<MeloFile>();

        string p = ctx.assetPath.Replace('\\', '/');
        file.ProjectPath = p.StartsWith("Assets/") ? p.Substring("Assets/".Length) : p;

        var icon = AssetDatabase.LoadAssetAtPath<Texture2D>(IconPath);
        ctx.AddObjectToAsset("melo", file, icon);
        ctx.SetMainObject(file);
    }
}
