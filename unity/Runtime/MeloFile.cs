using UnityEngine;

/// <summary>a .melo as an asset. drag it onto a MeloSource field and pass it to Melo.Load, like an AudioClip but for songs</summary>
public sealed class MeloFile : ScriptableObject
{
    /// <summary>path under Assets, e.g. "MelowriteProjects/Battle/music.melo". the importer fills it in</summary>
    public string ProjectPath;
}
