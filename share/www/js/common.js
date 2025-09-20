"use strict";

//	CONSTANTS/ENUMERATIONS

const TableLayout = Object.freeze({
	VertFirst:1,
	WrapHorz:2,
	Detailed:3
});

////////////////////////////////////////////////////////////////////////////////

function extractLeafData(){
	
	var url = new URL(window.location.toString());
	url.pathname = "/api/leaf";
	return fetchJSON(url);
};


function fetchJSON(url) {
    return fetch(url)
        .then(response => response.json())
        .catch((error) => {
            console.log(error);
        });
}

function pathRewrite(url, path){
	var ret = new URL(url.toString());
	ret.pathname = path;
	return ret;
}

function thumbnail(x, icon, obj = false)
{
    var img	= document.createElement("img");
    var url = pathRewrite(window.location, "/thumbnail");
    url.search = "id=" + icon;
    img.setAttribute("src", url.toString());
    img.setAttribute("class", "icon");
    x.appendChild(img);
}


////////////////////////////////////////////////////////////////////////////////

class Obj {
	constructor(pn, arg=false)
	{
        if(arg == false){
            this.id     = 0;
            this.key	= '';
            this.data   = null;
        } else if(arg instanceof Number){
            this.id     = arg;
            this.key	= '';
            this.data   = null;
            
            // Fetch TODO
        } else if(arg instanceof String){
            this.id     = 0;
            this.key	= arg;
            this.data   = null;

            // Fetch TODO
            
        } else {
            //  Assume it's a data structure
            this.id     = arg.id;
            this.key    = arg.key;
            this.data   = arg;
        }
	}
    
    card(here) {
        var table   = document.createElement("table");
        here.appendChild(table);
        var tr      = document.createElement("tr");
        table.appendChild(tr);
        var cname   = document.createElement("td");
        tr.appendChild(cname);
        var name    = cname
                        .appendChild(document.createElement("b"))
                        .appendChild(document.createElement('div'));
        name.setAttribute('id', `name${this.id}`);
        
        var u = this.url();
        if(u != false){
            name    = name.appendChild(document.createElement("a"));
            name.setAttribute('href', u);
        }
        
        name.innerText  = this.title();
        
        var ico = this.icon();
        
        if(ico != 0){
            var cicon   = document.createElement("td");
            cicon.setAttribute("rowspan", "3");
            cicon.setAttribute("width", "70");
            cicon.setAttribute("align", "center");
            tr.appendChild(cicon);
            thumbnail(cicon, ico);
        }
        
        tr      = document.createElement("tr");
        table.appendChild(tr);
        table.setAttribute('border', '1');
        table.setAttribute('width', '100%');
        
		var key			= tr.appendChild(document.createElement("pre"))
						 .appendChild(document.createElement("div"));
		key.setAttribute('id', `key${this.id}`);
		key.innerText   = this.key;
        
		var brief 		= tr .appendChild(document.createElement("small"))
							 .appendChild(document.createElement("i"))
							 .appendChild(document.createElement("div"));
		brief.setAttribute('id', `brief${this.id}`);
		brief.innerText	= this.brief();
    }
    
    icon(){
        return 0;
    }
    
    title(){
        return this.key;
    }
    
    brief(){
        return "";
    }
    
    url(){
        return false;
    }
};

class Atom extends Obj {
	constructor(arg = false)
	{
		super("/api/atom", arg);
	}
};

class Class extends Obj {
	constructor(arg = false)
	{
		super("/api/class", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.name;
    }
    
    brief(){
        return this.data.brief;
    }
};

class Character extends Obj {
	constructor(arg = false)
	{
		super("/api/character", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.name;
    }
    
    brief(){
        return this.data.brief;
    }
};


class Directory extends Obj {
	constructor(arg = false)
	{
		super("/api/directory", arg);
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

class Game extends Obj {
	constructor(arg = false)
	{
		super("/api/game", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.title;
    }
    
    brief(){
        return this.data.brief;
    }
};

class Leaf extends Obj {
	constructor(arg = false)
	{
		super("/api/leaf", arg);
	}

    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.title;
    }
    
    brief(){
        return this.data.brief;
    }
};

class Tag extends Obj {
	constructor(arg = false)
	{
		super("/api/tag", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.name;
    }
    
    brief(){
        return this.data.brief;
    }
};


////////////////////////////////////////////////////////////////////////////////

function toAtoms(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Atom(x));
    return ret;
}

function toCharacters(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Character(x));
    return ret;
}

function toClasses(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Class(x));
    return ret;
}

function toGames(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Game(x));
    return ret;
}

function toLeafs(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Leaf(x));
    return ret;
}

function toTags(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Tag(x));
    return ret;
}

function vertFirstCardTable(table, items, cols=3){
    const rcnt	= Math.trunc((items.length + cols - 1)/cols);
    for(var r=0;r<rcnt;++r){
        var trow	= document.createElement('tr');
        table.appendChild(trow);
        for(var c=0;c<cols;++c){
            var idx	= r+c*rcnt;
            if(idx >= items.length)
                continue;
            var tdata   = document.createElement('td');
            trow.append(tdata);
            items[idx].card(tdata);
        }
    }
}

function wrapHorzCardTable(table, items, cols=3){
    const rcnt	= Math.trunc((items.length + cols - 1)/cols);
    for(var r=0;r<rcnt;++r){
        var trow	= document.createElement('tr');
        table.appendChild(trow);
        for(var c=0;c<cols;++c){
            var idx	= c+r*cols;
            if(idx >= items.length)
                continue;
            var tdata   = document.createElement('td');
            trow.append(tdata);
            items[idx].card(tdata);
        }
    }
}
