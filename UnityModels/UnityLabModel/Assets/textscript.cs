using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class textscript : MonoBehaviour {
	public string disText;
	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		RaycastHit hit;

		Vector3 forward = transform.TransformDirection (Vector3.forward) * 1;
		Debug.DrawRay (transform.position, forward, Color.green);

		if (Physics.Raycast (transform.position, (forward), out hit, 4)) {
			if (hit.collider.gameObject.tag == "textBox") {
				GUI.Box (new Rect (0, 50, Screen.width, Screen.height - 50), disText);
				print("hahahhaahhaahahh");
			}
		}
	}
}