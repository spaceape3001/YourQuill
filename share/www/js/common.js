"use strict";

//	CONSTANTS/ENUMERATIONS

const TableLayout = Object.freeze({
	VertFirst:1,
	WrapHorz:2,
	Detailed:3
});

////////////////////////////////////////////////////////////////////////////////

function pathRewrite(url, path){
	var ret = new URL(url.toString());
	ret.pathname = path;
	return ret;
}

////////////////////////////////////////////////////////////////////////////////

function fetchJSON(url) {
    return fetch(url)
        .then(response => response.json())
        .catch((error) => {
            console.log(error);
        });
}

function extractLeafData(){
	
	var url = new URL(window.location.toString());
	url.pathname = "/api/leaf";
	return fetchJSON(url);
};


