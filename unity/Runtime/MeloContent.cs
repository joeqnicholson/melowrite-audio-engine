using System;
using System.IO;
using UnityEngine;

namespace Melowrite.Audio
{
    public static class MeloContent
    {
        public const string BuildSubfolder = "Melowrite";

        public static string Resolve(string assetRelativePath)
        {
            if (string.IsNullOrEmpty(assetRelativePath)) return null;
            string rel = Normalize(assetRelativePath);

#if UNITY_EDITOR
            string inAssets = Path.Combine(Application.dataPath, rel);
            if (File.Exists(inAssets)) return inAssets;
#endif
            string mirrored = Path.Combine(Application.streamingAssetsPath, BuildSubfolder, rel);
            if (File.Exists(mirrored)) return mirrored;

            return null;
        }

        public static string Normalize(string p)
        {
            if (string.IsNullOrEmpty(p)) return p;
            p = p.Replace('\\', '/').TrimStart('/');
            if (p.StartsWith("Assets/", StringComparison.OrdinalIgnoreCase))
                p = p.Substring("Assets/".Length);
            return p;
        }
    }
}
