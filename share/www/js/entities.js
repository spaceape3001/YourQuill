
class Entity extends Obj {
	constructor(arg = false)
	{
		super("/api/entity", arg);
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

function toEntities(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Entity(x));
    return ret;
}
