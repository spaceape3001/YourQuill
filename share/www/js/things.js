
class Thing extends Obj {
	constructor(arg = false)
	{
		super("/api/thing", arg);
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


function toThings(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Thing(x));
    return ret;
}
