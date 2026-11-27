using System;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;
using Melowrite;

// copies every .melo and its sample folders into StreamingAssets for the build, then cleans up after
public sealed class MelowriteBuildProcessor : IPreprocessBuildWithReport, IPostprocessBuildWithReport
{
    public int callbackOrder => 0;

    private static readonly string[] AssetSubfolders = { "Samples", "Soundfonts", "Impulses", "Presets" };

    private static bool createdStreamingAssets;

    public void OnPreprocessBuild(BuildReport report)
    {
        string mirrorRoot = MirrorRoot();

        if (Directory.Exists(mirrorRoot))
            Directory.Delete(mirrorRoot, true);

        createdStreamingAssets = !Directory.Exists(Application.streamingAssetsPath);

        List<string> folders = FindProjectFolders();
        int projects = 0, files = 0;
        foreach (string folder in folders)
        {
            string relFolder = RelativeToAssets(folder);
            string dest = string.IsNullOrEmpty(relFolder) ? mirrorRoot : Path.Combine(mirrorRoot, relFolder);
            Directory.CreateDirectory(dest);

            foreach (string melo in Directory.GetFiles(folder, "*.melo", SearchOption.TopDirectoryOnly))
            {
                CopyFile(melo, Path.Combine(dest, Path.GetFileName(melo)));
                files++;
                projects++;
            }

            foreach (string sub in AssetSubfolders)
            {
                string srcSub = Path.Combine(folder, sub);
                if (Directory.Exists(srcSub))
                    files += CopyTree(srcSub, Path.Combine(dest, sub));
            }
        }

        if (projects == 0)
        {
            Cleanup();
            return;
        }

        AssetDatabase.Refresh();
        Debug.Log($"[Melowrite] Bundled {projects} project(s), {files} file(s) into StreamingAssets/{MeloContent.BuildSubfolder}/ for the build.");
    }

    public void OnPostprocessBuild(BuildReport report) => Cleanup();

    private static void Cleanup()
    {
        string mirrorRoot = MirrorRoot();
        try
        {
            if (Directory.Exists(mirrorRoot))
                Directory.Delete(mirrorRoot, true);
            File.Delete(mirrorRoot + ".meta");

            if (createdStreamingAssets && Directory.Exists(Application.streamingAssetsPath)
                && Directory.GetFileSystemEntries(Application.streamingAssetsPath).Length == 0)
            {
                Directory.Delete(Application.streamingAssetsPath);
                File.Delete(Application.streamingAssetsPath + ".meta");
            }
            AssetDatabase.Refresh();
        }
        catch (Exception ex)
        {
            Debug.LogWarning($"[Melowrite] Could not fully clean the build mirror at {mirrorRoot}: {ex.Message}");
        }
    }

    private static string MirrorRoot()
        => Path.Combine(Application.streamingAssetsPath, MeloContent.BuildSubfolder);

    private static List<string> FindProjectFolders()
    {
        HashSet<string> folders = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
        foreach (string melo in Directory.GetFiles(Application.dataPath, "*.melo", SearchOption.AllDirectories))
        {
            string dir = Path.GetDirectoryName(melo);
            if (dir != null && !dir.Replace('\\', '/').Contains("/StreamingAssets/" + MeloContent.BuildSubfolder))
                folders.Add(dir);
        }
        return new List<string>(folders);
    }

    private static string RelativeToAssets(string absFolder)
    {
        string data = Application.dataPath.Replace('\\', '/').TrimEnd('/');
        string f = absFolder.Replace('\\', '/').TrimEnd('/');
        if (string.Equals(f, data, StringComparison.OrdinalIgnoreCase)) return "";
        if (f.StartsWith(data + "/", StringComparison.OrdinalIgnoreCase))
            return f.Substring(data.Length + 1);
        return Path.GetFileName(f);
    }

    private static int CopyTree(string srcDir, string dstDir)
    {
        int count = 0;
        Directory.CreateDirectory(dstDir);
        foreach (string file in Directory.GetFiles(srcDir))
        {
            if (file.EndsWith(".meta", StringComparison.OrdinalIgnoreCase)) continue;
            CopyFile(file, Path.Combine(dstDir, Path.GetFileName(file)));
            count++;
        }
        foreach (string sub in Directory.GetDirectories(srcDir))
            count += CopyTree(sub, Path.Combine(dstDir, Path.GetFileName(sub)));
        return count;
    }

    private static void CopyFile(string src, string dst)
    {
        Directory.CreateDirectory(Path.GetDirectoryName(dst));
        File.Copy(src, dst, true);
    }
}
