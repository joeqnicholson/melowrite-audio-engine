using UnityEngine;
using Melowrite;

[AddComponentMenu("Melowrite/Melo Listener")]
public class MeloListener : MonoBehaviour
{
    public float minDistance = 1f;
    public float falloff = 1f;

    void Update()
    {
        Vector3 p = transform.position, f = transform.forward, u = transform.up;
        Melo.SetSpatialListener(p.x, p.y, -p.z, f.x, f.y, -f.z, u.x, u.y, -u.z);
        Melo.SetSpatialDefaults(minDistance, falloff, 1f, MeloSpatialMode.ThreeD);
    }
}
