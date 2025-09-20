
class Event extends Obj {
	constructor(arg = false)
	{
		super("/api/event", arg);
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

function toEvents(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Event(x));
    return ret;
}
