using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class rayCastObj : MonoBehaviour {
	bool doorStatus = false;
	bool glassessOn = false;
	public GameObject explosion;
	public GameObject fire;
	public string disText;
	public Transform target;
	public GameObject onFace;
	public GameObject eyeBleedObject;
	public GameObject spray1ani;
	bool spray1Played = false;
	bool showText = false;
	bool screamPlayed = false;
	public GameObject screamObj;
	//Rect textArea = new Rect(0,0,Screen.width, Screen.height);
	//public GameObject spray2ani;

	void Start(){
	}

	// Update is called once per frame
	void Update () {
		RaycastHit hit;
		float theDistance;
		GameObject dDoor;

		Vector3 forward = transform.TransformDirection (Vector3.forward) * 1;
		Debug.DrawRay (transform.position, forward, Color.green);

		if (Physics.Raycast (transform.position, (forward), out hit, 4)) {
			if (hit.collider.gameObject.tag == "door" && doorStatus == false) {
				hit.collider.gameObject.GetComponent<Animation> ().Play ("doorOpen");
				doorStatus = true;

				//if (showText == false) {
				//	GUI.Label(textArea,"Here is a block of text\nlalalala\nanother line\nI could do this all day!");
				//	showText = true;
				//}
			}
		}

		if (Physics.Raycast (transform.position, (forward), out hit, 2)) {
			if (hit.collider.gameObject.tag == "tableGlass") {
				Destroy (hit.collider.gameObject);
				//print ("wearing glassess");
				onFace.GetComponent<Animation> ().Play ("glassessOnFace");
				glassessOn = true;	
			}

			if (hit.collider.gameObject.tag == "chemicalReactor" || hit.collider.gameObject.tag == "tableExplo" || hit.collider.gameObject.tag == "flask") {
				Instantiate (explosion, transform.position, transform.rotation);
				if (glassessOn == false) {
					eyeBleedObject.GetComponent<Animation> ().Play ("bloodLine");
					screamObj.GetComponent<AudioSource> ().Play ();
				} 
				if (glassessOn == true && spray1Played == false) {
					print ("spraying animation");
					spray1ani.GetComponent<Animation> ().Play ("spot1new");
					spray1Played = true;
				}
			}
		}
	}
}


//videoPlayer.Play ();
//GameObject camera = GameObject.Find("Quad 9");
// var videoPlayer = camera.AddComponent<UnityEngine.Video.VideoPlayer>();

/**
 *  
 
 
 function OnGUI()
 {
 
     if(showText)
     {
 
     GUI.Label(textArea,"Here is a block of text\nlalalala\nanother line\nI could do this all day!");
 
     }
 
 }
**/