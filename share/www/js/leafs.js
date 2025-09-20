
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

function toLeafs(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Leaf(x));
    return ret;
}
