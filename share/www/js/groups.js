
class Group extends Obj {
	constructor(arg = false)
	{
		super("/api/group", arg);
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

function toGroups(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Group(x));
    return ret;
}
