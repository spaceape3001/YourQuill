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

class Obj {
	id = 0;
	key	= '';
	data;
	
	constructor(pn, arg=false)
	{
	}
};

class Directory extends Obj {
	constructor(arg = false)
	{
		super("/api/directory", arg);
	}
};


class Tag extends Obj {
	constructor(arg = false)
	{
		super("/api/tag", arg);
	}
};

class Leaf extends Obj {
	constructor(arg = false)
	{
		super("/api/leaf", arg);
	}
};

class Document extends Obj {
	constructor(arg = false)
	{
		super("/api/document", arg);
	}
};

class Fragment extends Obj {
	constructor(arg = false)
	{
		super("/api/fragment", arg);
	}
};

class Folder extends Obj {
	constructor(arg = false)
	{
		super("/api/folder", arg);
	}
};



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


